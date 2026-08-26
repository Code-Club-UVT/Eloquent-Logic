#ifndef SAT_ALGORITHMS_DPLL_RAND_H
#define SAT_ALGORITHMS_DPLL_RAND_H

#include "common.h"
#include "sat_listener.hpp"
#include <memory>

SatState dpll_rand(ClauseSet,
                   const std::shared_ptr<eloquent::logic::sat_listener> &);

#endif // SAT_ALGORITHMS_DPLL_RAND_H
