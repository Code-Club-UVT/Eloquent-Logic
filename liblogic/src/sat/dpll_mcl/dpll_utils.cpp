#include "dpll_utils.h"
#include <cmath>

using eloquent::logic::sat_listener;

Sat::Sat(int nrClauses, int nrMax, const ClauseSet &clauses) {
    this->nrClauses = nrClauses;
    this->nrMax = nrMax;
    this->clauses = clauses;
}

Sat::Sat(const Sat &s) {
    this->clauses = s.clauses;
    this->nrClauses = s.nrClauses;
    this->nrMax = s.nrMax;
}

void max_freq(Literal &max, int &freq, const Sat &s,
              const std::shared_ptr<sat_listener> &listener) {
    max = 0;
    freq = 0;

    std::vector<int> f(s.nrMax + 1, 0);

    for (const auto &clause : s.clauses) {
        for (Literal lit : clause) {
            Literal k = std::abs(lit);
            if (k <= s.nrMax) {
                f[k]++;
                if (f[k] > freq) {
                    max = k;
                    freq = f[k];
                }
            }
        }
    }
    listener->didChooseBranchingLiteral(max, freq);
}

void solve_chosen_clause(Sat &s, bool &no_vid_clause, Literal clause,
                         const std::shared_ptr<sat_listener> &listener) {
    ClauseSet new_clauses;

    for (const auto &c : s.clauses) {
        if (c.contains(clause)) {
            s.nrClauses--;
            listener->didSatisfyClauseByUnitLiteral(c, clause);
            continue;
        }

        if (c.contains(-clause)) {
            Clause temp = c;
            temp.erase(-clause);

            if (temp.empty()) {
                no_vid_clause = false;
                listener->didDeriveEmptyClauseFromUnitPropagation(clause);
                listener->didDetectConflict();
                return;
            }
            new_clauses.insert(temp);
            listener->didReduceClauseByUnitLiteral(c, temp, clause);
        } else {
            new_clauses.insert(c);
        }
    }

    s.clauses = std::move(new_clauses);
}

void solve_unit_clauses(Sat &s, bool &no_vid_clause,
                        const std::shared_ptr<sat_listener> &listener) {
    bool no_change = true;
    while (no_change) {
        no_change = false;

        for (const auto &clause : s.clauses) {
            if (clause.size() == 1) {
                no_change = true;
                Literal unit_lit = *clause.begin();
                listener->didFindUnitLiteral(unit_lit);
                solve_chosen_clause(s, no_vid_clause, unit_lit, listener);
                break;
            }
        }
        if (!no_vid_clause)
            break;
    }
}

bool det_satisfiability(Sat &s, const std::shared_ptr<sat_listener> &listener) {
    bool no_vid_clause = true;

    solve_unit_clauses(s, no_vid_clause, listener);
    if (s.clauses.empty()) {
        listener->didFindSatisfyingAssignment();
        return true;
    }
    if (!no_vid_clause) {
        listener->didBacktrack();
        return false;
    }

    Literal max_lit = 0;
    int freq = 0;
    max_freq(max_lit, freq, s, listener);

    if (freq <= 1) {
        listener->didFindSatisfyingAssignment();
        return true;
    }

    Sat copy(s);
    listener->didEnterBranch(max_lit, true);
    solve_chosen_clause(copy, no_vid_clause, max_lit, listener);

    if (no_vid_clause && det_satisfiability(copy, listener))
        return true;

    no_vid_clause = true;
    listener->didBacktrack();
    listener->didEnterBranch(-max_lit, false);
    solve_chosen_clause(s, no_vid_clause, -max_lit, listener);

    if (!no_vid_clause) {
        listener->didBacktrack();
        return false;
    }
    return det_satisfiability(s, listener);
}
