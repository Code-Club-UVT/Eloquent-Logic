#ifndef ELOQUENTLOGIC_AGENT_SAT_CONVERT_HPP
#define ELOQUENTLOGIC_AGENT_SAT_CONVERT_HPP

#include <common.h>

#include "common_t.hpp"

// Conversion functions from the library's live SAT types (Clause,
// ClauseSet) to the plain, JSON-serializable DTOs in
// logic_agent::types::sat. These are the only places in
// logic_agent/types/sat that depend on liblogic's real headers.
// Literal needs no converter: it and literal_t are both std::int64_t.
namespace logic_agent::types::sat
{
    clause_t to_dto(const Clause& clause);

    clause_set_t to_dto(const ClauseSet& clauses);
}

#endif //ELOQUENTLOGIC_AGENT_SAT_CONVERT_HPP
