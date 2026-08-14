#include <fstream>
#include <vector>
#include <chrono>

#include "resolution_naive_first_fit.h"

using eloquent::logic::sat_listener;

static ClauseSet clauses;

constexpr std::size_t THRESHOLD = 71000000;
namespace fs = std::filesystem;


std::pair<bool,int64_t> can_join(const Clause& c1, const Clause& c2) {
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
Clause join(const Clause& c1, const Clause& c2, const Literal l) {
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
[[nodiscard]] SatState resolution(ClauseSet& cs, const std::shared_ptr<sat_listener>& listener) {
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
                    listener->didExceedClauseThreshold(cs.size());
                    listener->didConcludeUnknown();
                    return SatState::UNKNOWN;
                }
                auto result = can_join(*i,*j);
                if (result.first == false) continue;

                listener->didSelectResolutionCandidates(*i, *j);
                auto new_clause = join(*i,*j,result.second);
                listener->didComputeResolvent(*i, *j, result.second, new_clause);

                if (new_clause.empty()) {
                    listener->didDeriveEmptyClauseFromResolution();
                    listener->didConcludeUnsat();
                    return SatState::UNSAT;
                }

                if (cs.contains(new_clause)) {
                    listener->didSkipDuplicateResolvent(new_clause);
                    continue;
                }

                canMakeNewClause = true;
                cs.emplace(new_clause);
                listener->didAddResolvent(new_clause);
            }
        }
    }
    while (canMakeNewClause);
    listener->didReachResolutionSaturation();
    listener->didConcludeSat();
    return SatState::SAT;
}
SatState res(ClauseSet c, const std::shared_ptr<sat_listener>& listener) {
    listener->didStart();
    auto result = resolution(c, listener);
    listener->didFinish();
    return result;
}
