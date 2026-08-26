#include "convert.hpp"

namespace logic_agent::types::sat {
clause_t to_dto(const Clause &clause) {
    return clause_t(clause.begin(), clause.end());
}

clause_set_t to_dto(const ClauseSet &clauses) {
    clause_set_t dto;
    dto.reserve(clauses.size());
    for (const auto &clause : clauses) {
        dto.push_back(to_dto(clause));
    }
    return dto;
}
} // namespace logic_agent::types::sat
