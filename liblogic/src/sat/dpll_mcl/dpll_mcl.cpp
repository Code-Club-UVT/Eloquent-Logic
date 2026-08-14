#include <fstream>
#include <filesystem>
#include <sstream>
#include <chrono>
#include <cmath>
#include <algorithm>
#include "dpll_utils.h"
#include "dpll_mcl.h"

using eloquent::logic::sat_listener;

namespace fs = std::filesystem;

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
[[nodiscard]] static ClauseSet read_clauses(const char *file) {
    std::ifstream f(file);
    f.tie(nullptr);

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

SatState dpll_mcl(ClauseSet c, const std::shared_ptr<sat_listener>& listener) {
    listener->didStart();
    SatState state = SatState::UNKNOWN;
    int clause_total_count = c.size();
    int lit_total_count = get_lit_total_count(c);
    Sat s(clause_total_count, lit_total_count, c);

    state = det_satisfiability(s, listener) == true ? SatState::SAT : SatState::UNSAT;
    if (state == SatState::SAT) {
        listener->didConcludeSat();
    } else {
        listener->didConcludeUnsat();
    }

    listener->didFinish();
    return state;
}
