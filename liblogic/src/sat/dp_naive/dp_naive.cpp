#include <vector>
#include <chrono>
#include <map>
#include <fstream>
#include "dp_naive.h"

using eloquent::logic::sat_listener;

namespace fs = std::filesystem;

#pragma GCC optimize("O3,fast-math,unroll-loops")

static ClauseSet clauses;
static std::size_t max_clauses = 0;

constexpr std::size_t THRESHOLD = 71000000;

void analyse(const Clause& c, HeuristicsDB_DP& db) {
    for (auto& literal: c) {
        ++db[literal];
        db[literal].set_literal(literal);
        db[-literal].set_literal(-literal);
    }
}

bool resolution(ClauseSet& cs, HeuristicsDB_DP& hdb, std::set<Literal>& single_polarity_literals, std::set<Literal>& single_literals, SatState& sat_result, const std::shared_ptr<sat_listener>& listener);
static std::pair<bool,int64_t> can_join(const Clause& c1, const Clause& c2) {
    size_t pairs = 0;
    int64_t lit = 0;
    for (auto& literal: c1) {
        if (c2.contains(literal*-1)) {
            ++pairs;
            lit = literal;
        }
    }
    return {pairs == 1,lit};
}

void process_clause_removal(HeuristicsDB_DP &db, const Clause& clause) {
    for (auto &c_lit: clause)
        --db[c_lit];
}

bool one_literal_clause_rule(ClauseSet &cs, std::set<Literal>& single_literals, SatState &result, HeuristicsDB_DP& db, const std::shared_ptr<sat_listener>& listener) {
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
                    listener->didConcludeSat();
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
                    listener->didConcludeUnsat();
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

bool single_polarity_rule(ClauseSet &cs, HeuristicsDB_DP &db, std::set<Literal>& single_polarity_literals, SatState &result, const std::shared_ptr<sat_listener>& listener) {
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
                    listener->didConcludeSat();
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

SatState davis_putnam(ClauseSet& cs, HeuristicsDB_DP& db, const std::shared_ptr<sat_listener>& listener) {

    SatState result = SatState::UNKNOWN;
    std::set<Literal> single_polarity_literals;
    std::set<Literal> single_literals;
    for (const auto& clause: cs) {
        if (clause.size() == 1) {
            single_literals.emplace(*clause.begin());
        }
        for (const auto& literal: clause) {
            if (db[-literal] == 0) {
                single_polarity_literals.emplace(literal);
            }
        }
    }

    while (true) {
        if (one_literal_clause_rule(cs, single_literals, result, db, listener)) return result;
        if (single_polarity_rule(cs, db, single_polarity_literals, result, listener)) return result;
        if (resolution(cs,db,single_polarity_literals,single_literals,result, listener)) return result;
    }
}

static Clause join(const Clause& c1, const Clause& c2, const Literal l) {
    Clause c12;
    for (auto& lit : c1) {
        c12.emplace(lit);
    }
    for (auto& lit : c2) {
        c12.emplace(lit);
    }
    c12.erase(l);
    c12.erase(-l);
    return c12;
}
bool resolution(ClauseSet& cs, HeuristicsDB_DP& hdb, std::set<Literal>& single_polarity_literals, std::set<Literal>& single_literals, SatState& sat_result, const std::shared_ptr<sat_listener>& listener) {
    bool canMakeNewClause = false;
    do
    {
        canMakeNewClause = false;
        size_t iindex = 0,jindex=0;
        for (auto i = cs.begin(); i != cs.end(); ++i,++iindex) {
            auto j = i;
            std::advance(j,1);

            for (jindex=iindex+1; j != cs.end(); ++j,++jindex) {
                if (cs.size() >= THRESHOLD) {
                    sat_result = SatState::UNKNOWN;
                    listener->didExceedClauseThreshold(cs.size());
                    listener->didConcludeUnknown();
                    return true;
                }
                auto result = can_join(*i,*j);
                if (result.first == false) continue;

                listener->didSelectResolutionCandidates(*i, *j);
                auto new_clause = join(*i,*j,result.second);
                listener->didComputeResolvent(*i, *j, result.second, new_clause);

                if (new_clause.empty()) {
                    sat_result = SatState::UNSAT;
                    listener->didDeriveEmptyClauseFromResolution();
                    listener->didConcludeUnsat();
                    return true;
                }

                if (cs.contains(new_clause)) {
                    listener->didSkipDuplicateResolvent(new_clause);
                    continue;
                }

                for (const auto& lit: new_clause) {
                    ++hdb[lit];
                }
                bool should_repeat_dp = false;
                if (hdb[result.second] > 0 && hdb[-result.second] == 0) {
                    single_polarity_literals.emplace(result.second);
                    should_repeat_dp = true;
                }
                else if (hdb[-result.second] > 0 && hdb[result.second] == 0) {
                    single_polarity_literals.emplace(-result.second);
                    should_repeat_dp = true;
                }
                if (new_clause.size() == 1) {
                    single_literals.emplace(*new_clause.begin());
                    should_repeat_dp = true;
                }
                canMakeNewClause = true;
                cs.emplace(new_clause);
                listener->didAddResolvent(new_clause);
                //reset_flag = true;

                max_clauses = std::max(max_clauses,clauses.size());
                if (should_repeat_dp) //nu am rezolvat, dar am găsit ceva "interesant"
                    return false;

            }
        }
    }
    while (canMakeNewClause);
    sat_result = SatState::SAT;
    listener->didReachResolutionSaturation();
    listener->didConcludeSat();
    return true;
}
int get_lit_total_count(const ClauseSet& c) {
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
void build_heuristics_db(const ClauseSet& c, HeuristicsDB_DP& db, const std::shared_ptr<sat_listener>& listener) {
    int max_lit = get_lit_total_count(c);
    db.resize(max_lit);

    for (const auto& clause : c) {
        analyse(clause, db);
    }
    listener->didBuildHeuristicsDatabase(c);
}
SatState dp(ClauseSet c, const std::shared_ptr<sat_listener>& listener) {
    listener->didStart();
    HeuristicsDB_DP db;
    build_heuristics_db(c, db, listener);
    max_clauses = clauses.size();

    SatState result = davis_putnam(c, db, listener);
    listener->didFinish();
    return result;
}
