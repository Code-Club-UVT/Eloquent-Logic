#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdint>
#include <vector>
#include <chrono>
#include <memutils.h>
#include <csignal>
#include "resolution_naive_first_fit.h"


ClauseSet clauses;

constexpr std::size_t THRESHOLD = 71000000;
namespace fs = std::filesystem;

[[nodiscard]] ClauseSet read_clauses(const char *file) {
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
            is>>clause_count>>lit_total_count;
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
[[nodiscard]] SatState resolution(ClauseSet& cs) {
    bool canMakeNewClause = false;
    do
    {
        canMakeNewClause = false;
        size_t iindex = 0,jindex=0;
        for (auto i = cs.begin(); i != cs.end(); ++i,++iindex) {
            auto j = i;
            std::advance(j,1);

            for (jindex=iindex+1; j != cs.end(); ++j,++jindex) {
                if (cs.size() >= THRESHOLD)
                    return SatState::UNKNOWN;
                auto result = can_join(*i,*j);
                if (result.first == false) continue;

                auto new_clause = join(*i,*j,result.second);

                if (new_clause.empty())
                    return SatState::UNSAT;

                if (cs.contains(new_clause)) continue;

                canMakeNewClause = true;
                cs.emplace(new_clause);
            }
        }
    }
    while (canMakeNewClause);
    return SatState::SAT;
}
SatState res(ClauseSet c) {
    return resolution(c);
}