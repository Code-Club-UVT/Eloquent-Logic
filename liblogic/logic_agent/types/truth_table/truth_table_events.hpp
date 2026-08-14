#ifndef ELOQUENTLOGIC_AGENT_TRUTH_TABLE_EVENTS_HPP
#define ELOQUENTLOGIC_AGENT_TRUTH_TABLE_EVENTS_HPP

#include <string>

#include <nlohmann/json.hpp>

#include "../core/uuid_t.hpp"

// DTOs for eloquent::logic::truth_table_listener_t
// (src/truth_table/truth_table_listener_t.hpp) callback parameters:
//   - didFindUnknownVariable(string_view name)       -> unknown_variable_event_t
//   - didSetVariable(string_view name, bool value)   -> variable_assignment_event_t
//   - didComputeSubexpression(UUID node_id, bool value) -> subexpression_result_event_t
namespace logic_agent::types::truth_table
{
    struct unknown_variable_event_t
    {
        std::string name;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(unknown_variable_event_t, name);

    struct variable_assignment_event_t
    {
        std::string name;
        bool value = false;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(variable_assignment_event_t, name, value);

    struct subexpression_result_event_t
    {
        uuid_t node_id;
        bool value = false;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(subexpression_result_event_t, node_id, value);
}

#endif //ELOQUENTLOGIC_AGENT_TRUTH_TABLE_EVENTS_HPP
