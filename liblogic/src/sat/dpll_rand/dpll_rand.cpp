/**
 * Pentru acest program vom folosi strategia de alegere 'la întâmplare', numită și RAND
*/
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <map>
#include <random>
#include <stack>
#include "dpll_rand.h"

using eloquent::logic::sat_listener;

namespace fs = std::filesystem;

#pragma GCC optimize("O3,fast-math,unroll-loops")

static std::size_t max_clauses = 0;
class HeuristicEntryDPLL {
    Literal literal;
    size_t occurrences;
public:
    explicit HeuristicEntryDPLL(Literal l, size_t occurences = 0)
        : literal(l),
          occurrences(occurences) {
    }
    HeuristicEntryDPLL() {
        literal = 0;
        occurrences = 0;
    }

    friend auto operator<=>(const HeuristicEntryDPLL &lhs, size_t rhs) {
        return lhs.occurrences <=> rhs;
    }
    HeuristicEntryDPLL operator++(int) {
        HeuristicEntryDPLL e( *this);
        this->occurrences++;
        return e;
    }
    HeuristicEntryDPLL &operator++() {
        this->occurrences++;
        return *this;
    }

    [[nodiscard]] Literal get_literal() const {
        return literal;
    }

    [[nodiscard]] size_t get_occurrences() const {
        return occurrences;
    }
    void set_literal(const Literal l) {
        this->literal = l;
    }

    HeuristicEntryDPLL &operator--() {
        if (occurrences != 0)
            this->occurrences--;
        else [[unlikely]]
            std::cerr<<"Decrementare ilegală pentru "<<this->literal<<'\n';
        return *this;
    }
    HeuristicEntryDPLL operator--(int) {
        const HeuristicEntryDPLL e = *this;
        if (occurrences != 0) {
            this->occurrences--;
        }
        else [[unlikely]]
            std::cerr<<"Decrementare ilegală pentru "<<this->literal<<'\n';

        return e;
    }

    friend bool operator==(const HeuristicEntryDPLL& lhs, size_t i) {
        return lhs.occurrences == i;
    }
};

class HeuristicsDB_DPLL {
    std::vector<HeuristicEntryDPLL> vector;
    std::vector<Literal> existing_literals;
    size_t size = 0;

public:
    explicit HeuristicsDB_DPLL(const size_t size): size(size) {
        this->vector.resize(2*size+2);
        this->existing_literals.resize(0);
    }
    explicit HeuristicsDB_DPLL() = default;
    HeuristicEntryDPLL& operator[](Literal lit) {
        if (lit < 0)
            return this->vector[2*std::abs(lit)+1];
        return this->vector[2*lit];
    }
    auto begin() {
        return vector.begin();
    }
    auto end() {
        return vector.end();
    }
    void resize(const size_t new_size) {
        this->vector.resize(2*new_size+2);
        this->size = new_size;
    }
    void poppulate_literal_record() {
        existing_literals.clear();
        for (auto i = 2; i< this->vector.size(); ++i)
            if (vector[i].get_occurrences() > 0)
                this->existing_literals.emplace_back(vector[i].get_literal());
    }
    [[nodiscard]] std::vector<Literal>& get_record() {
        return existing_literals;
    }
};
class ProblemContext {
    HeuristicsDB_DPLL db;
    ClauseSet cs;

public:
    [[nodiscard]] ProblemContext(const HeuristicsDB_DPLL &db, const ClauseSet &cs)
        : db(db),
          cs(cs) {
    }
    [[nodiscard]] HeuristicsDB_DPLL & get_db() {
        return db;
    }
    [[nodiscard]] ClauseSet & get_cs() {
        return cs;
    }
};
constexpr std::size_t THRESHOLD = 71000000;


void analyse(const Clause& c, HeuristicsDB_DPLL& db) {
    for (auto& literal: c) {
        ++db[literal];
        db[literal].set_literal(literal);
        db[-literal].set_literal(-literal);
    }
}
static int get_lit_total_count(const ClauseSet& c) {
    int lit_total_count = 0;

    for (const auto& clause : c) {
        if (clause.empty()) continue;

        int first_abs = std::abs(*clause.begin());
        int last_abs = std::abs(*clause.rbegin());

        int max_in_clause = std::max(first_abs, last_abs);
        if (max_in_clause > lit_total_count) {
            lit_total_count = max_in_clause;
        }
    }

    return lit_total_count;
}
void build_heuristics_db(const ClauseSet& c, HeuristicsDB_DPLL& db, const std::shared_ptr<sat_listener>& listener) {
    int max_lit = get_lit_total_count(c);

    db.resize(max_lit);

    for (const auto& clause : c) {
        analyse(clause, db);
    }

    db.poppulate_literal_record();
    listener->didBuildHeuristicsDatabase(c);
}
[[nodiscard]] static ClauseSet read_clauses(const char *file) {
    std::ifstream f(file);
    f.tie(nullptr);
    size_t clause_count=0, lit_total_count=0;
    ClauseSet clauses;
    while (!f.eof()) {
        std::string line;
        std::getline(f,line);
        if (line.empty()) break;
        if (line =="%" || line=="0") continue;
        while (line.starts_with('c')) {
            std::getline(f,line);
        }
        if (line.starts_with("p cnf ")) {
            line = line.substr(strlen("p cnf "));
            std::istringstream is(line);
            is>>lit_total_count>>clause_count;
            continue;
        }

        std::istringstream is(line);
        Clause c;
        int64_t lit = 0;
        while (is>>lit) {
            if (lit == 0) break;
            c.emplace(lit);
        }
        clauses.emplace(c);
    }
    f.close();
    return clauses;
}

void process_clause_removal(HeuristicsDB_DPLL &db, const Clause& clause) {
    for (auto &c_lit: clause)
        --db[c_lit];
}

bool one_literal_clause_rule(ClauseSet &cs, std::set<Literal>& single_literals, SatState &result, HeuristicsDB_DPLL& db, const std::shared_ptr<sat_listener>& listener) {
    while (!single_literals.empty()) {
        for (const auto& lit: single_literals) {
            listener->didFindUnitLiteral(lit);
            std::vector<Clause> satisfied;
            std::vector<Clause> to_reduce;

            for (const auto& clause : cs) {
                if (clause.contains(lit)) {
                    satisfied.push_back(clause);
                    continue;
                }
                if (clause.contains(-lit)) {
                    to_reduce.push_back(clause);
                }
            }

            for (const auto& clause : satisfied) {
                cs.erase(clause);
                listener->didSatisfyClauseByUnitLiteral(clause, lit);
                if (cs.empty()) {
                    result = SatState::SAT;
                    listener->didFindSatisfyingAssignment();
                    return true;
                }
                //std::cerr<<"Șterg clauza prin literal pur\n";
                process_clause_removal(db, clause);
            }

            for (const auto& clause : to_reduce) {
                cs.erase(clause);
                //std::cerr<<"Șterg complementarul prin literal pur\n";
                Clause reduced = clause;
                reduced.erase(-lit);
                if (reduced.empty()) {
                    result = SatState::UNSAT;
                    listener->didDeriveEmptyClauseFromUnitPropagation(lit);
                    listener->didDetectConflict();
                    return true;
                }
                --db[-lit];
                cs.emplace(reduced);
                listener->didReduceClauseByUnitLiteral(clause, reduced, lit);
            }
        }
        single_literals.clear();
        for (const auto& clause: cs) {
            if (clause.size() == 1) {
                single_literals.emplace(*clause.begin());
            }
        }
    }
    return false;
}

bool single_polarity_rule(ClauseSet &cs, HeuristicsDB_DPLL &db, std::set<Literal>& single_polarity_literals, SatState &result, const std::shared_ptr<sat_listener>& listener) {
    while (!single_polarity_literals.empty()) {
        for (const auto& literal: single_polarity_literals) {
            listener->didFindPureLiteral(literal);
            std::vector<Clause> to_erase;
            for (const auto& clause : cs) {
                if (clause.contains(literal)) {
                    to_erase.push_back(clause);
                }
            }
            for (const auto& clause : to_erase) {
                cs.erase(clause);
                //std::cerr<<"Șterg clauza prin literal cu aceiași polaritate\n";
                process_clause_removal(db, clause);
                listener->didEliminateClauseByPureLiteral(clause, literal);
                if (cs.empty()) {
                    result = SatState::SAT;
                    listener->didFindSatisfyingAssignment();
                    return true;
                }
            }
        }
        single_polarity_literals.clear();
        for (const auto& clause: cs) {
            for (const auto& literal: clause) {
                if (db[-literal] == 0) {
                    single_polarity_literals.emplace(literal);
                }
            }
        }
    }
    return false;
}

bool davis_putnam(ProblemContext& pc, SatState& result, const std::shared_ptr<sat_listener>& listener) {

    std::set<Literal> single_polarity_literals;
    std::set<Literal> single_literals;
    for (const auto& clause: pc.get_cs()) {
        if (clause.size() == 1) {
            single_literals.emplace(*clause.begin());
        }
        for (const auto& literal: clause) {
            if (pc.get_db()[-literal] == 0) {
                single_polarity_literals.emplace(literal);
            }
        }
    }

    if (one_literal_clause_rule(pc.get_cs(), single_literals, result, pc.get_db(), listener)) return true;
    if (single_polarity_rule(pc.get_cs(), pc.get_db(), single_polarity_literals, result, listener)) return true;
    return false;
}


Literal pick_literal(ProblemContext & ctx, const std::shared_ptr<sat_listener>& listener) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> distrib(0, ctx.get_db().get_record().size()-1);

    Literal l = ctx.get_db().get_record()[distrib(gen)];
    listener->didChooseRandomLiteral(l);
    return l;
}
SatState davis_putnam_logemann_loveland(const HeuristicsDB_DPLL &db, const ClauseSet &cs, const std::shared_ptr<sat_listener>& listener) {
    SatState result = SatState::UNKNOWN;
    std::stack<ProblemContext> context_stack;
    context_stack.emplace(db,cs);

    while (!context_stack.empty()) {
        auto c_context = context_stack.top();
        context_stack.pop();
        listener->didPopBranchContext();
        auto c_solved = davis_putnam(c_context,result, listener);
        c_context.get_db().poppulate_literal_record();
        if (c_solved && result == SatState::SAT) {
            listener->didConcludeSat();
            return SatState::SAT;
        }
        if (c_solved) {
            // this context's clause set contains an empty clause (result == UNSAT):
            // a dead branch, drop it and move on to the next one on the stack.
            listener->didBacktrack();
        }
        if (!c_solved){
            Literal l = pick_literal(c_context, listener);
            Clause literal({l});
            Clause notLiteral({-l});
            ProblemContext literal_ctx = c_context;
            ProblemContext negative_literal_ctx = c_context;
            literal_ctx.get_cs().emplace(literal);
            ++literal_ctx.get_db()[l];
            literal_ctx.get_db().poppulate_literal_record();

            negative_literal_ctx.get_cs().emplace(notLiteral);
            ++negative_literal_ctx.get_db()[-l];
            negative_literal_ctx.get_db().poppulate_literal_record();

            listener->didPushBranchContext(-l, false);
            context_stack.push(negative_literal_ctx);
            listener->didPushBranchContext(l, true);
            context_stack.push(literal_ctx);
        }

    }
    listener->didConcludeUnsat();
    return SatState::UNSAT;
}
SatState dpll_rand(ClauseSet clauses, const std::shared_ptr<sat_listener>& listener) {
    listener->didStart();
    HeuristicsDB_DPLL db;
    build_heuristics_db(clauses, db, listener);
    max_clauses = clauses.size();

    SatState result = davis_putnam_logemann_loveland(db, clauses, listener);
    listener->didFinish();
    return result;
}
