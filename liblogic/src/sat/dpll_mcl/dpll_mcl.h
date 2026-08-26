#ifndef SAT_ALGORITHMS_DPLL_MCL_CPP_H
#define SAT_ALGORITHMS_DPLL_MCL_CPP_H

#include "common.h"
#include "sat_listener.hpp"
#include <memory>

SatState dpll_mcl(ClauseSet,
                  const std::shared_ptr<eloquent::logic::sat_listener> &);

#endif // SAT_ALGORITHMS_DPLL_MCL_CPP_H
