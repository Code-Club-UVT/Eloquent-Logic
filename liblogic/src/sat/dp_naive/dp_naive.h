#ifndef SAT_ALGORITHMS_DP_NAIVE_H
#define SAT_ALGORITHMS_DP_NAIVE_H

#include "../Heuristics/HeuristicsDB_DP.h"
#include "sat_listener.hpp"
#include <memory>

SatState dp(ClauseSet, const std::shared_ptr<eloquent::logic::sat_listener> &);
int get_lit_total_count(const ClauseSet &);
void build_heuristics_db(
    const ClauseSet &, HeuristicsDB_DP &,
    const std::shared_ptr<eloquent::logic::sat_listener> &);
void analyse(const Clause &, HeuristicsDB_DP &);

#endif // SAT_ALGORITHMS_DP_NAIVE_H
