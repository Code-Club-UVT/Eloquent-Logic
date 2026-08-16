#ifndef ELOQUENTLOGIC_AGENT_MAPPER_EVENTS_HPP
#define ELOQUENTLOGIC_AGENT_MAPPER_EVENTS_HPP

#include <map>
#include <string>

#include <nlohmann/json.hpp>

#include "common_t.hpp"
#include "../feedback_base.hpp"

// DTOs for eloquent::logic::mapper_listener (src/sat/utilities/mapper_listener.hpp)
// callback parameters:
//   - did_map_literals(const std::map<std::string, Literal>&) -> literal_mapping_event_t
//   - did_map_clauses(const ClauseSet&)                        -> clause_set_event_t
// clause_set_event_t is also reused by sat_listener's
// didBuildHeuristicsDatabase, which carries the same ClauseSet shape.
namespace logic_agent::types::sat
{
    struct literal_mapping_event_t : public feedback_base
    {
        std::map<std::string, literal_t> mapping;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(literal_mapping_event_t, mapping);

    struct clause_set_event_t : public feedback_base
    {
        clause_set_t clauses;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(clause_set_event_t, clauses);
}

#endif //ELOQUENTLOGIC_AGENT_MAPPER_EVENTS_HPP
