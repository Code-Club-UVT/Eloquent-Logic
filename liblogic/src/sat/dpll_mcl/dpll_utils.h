#ifndef DPLL_UTILS_H
#define DPLL_UTILS_H

#include "common.h"
#include "sat_listener.hpp"
#include <cstdint>
#include <memory>
#include <set>
#include <vector>

using namespace std;

class Sat {
  int nrClauses;
  int nrMax;
  ClauseSet clauses;

public:
  Sat(int, int, const ClauseSet &);
  Sat(const Sat &);
  friend bool
  det_satisfiability(Sat &,
                     const std::shared_ptr<eloquent::logic::sat_listener> &);

private:
  friend void
  solve_chosen_clause(Sat &, bool &, Literal,
                      const std::shared_ptr<eloquent::logic::sat_listener> &);
  friend void
  solve_unit_clauses(Sat &, bool &,
                     const std::shared_ptr<eloquent::logic::sat_listener> &);
  friend void max_freq(Literal &, int &, const Sat &,
                       const std::shared_ptr<eloquent::logic::sat_listener> &);
};

#endif
