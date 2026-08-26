#ifndef ELOQUENTLOGIC_AGENT_LOGICAL_CONSEQUENCE_EVENTS_HPP
#define ELOQUENTLOGIC_AGENT_LOGICAL_CONSEQUENCE_EVENTS_HPP

#include <nlohmann/json.hpp>

#include "../feedback_base.hpp"

// DTO for eloquent::logic::logical_consequence_listener_t
// (src/logical_consequence/logical_consequence_listener_t.hpp) callback
// parameters that aren't already covered by a core type:
//   - not_logical_equivalence() -> no data, streamed via event_sink::emit_empty
//   - finalised_with_result(bool) -> result_event_t
namespace logic_agent::types::logical_consequence {
// finalised_with_result(result) -- whether Premises \models Conclusion holds
// (true) or not (false); see src/logical_consequence/logical_consequence.cpp.
struct result_event_t : public feedback_base {
    bool result = false;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(result_event_t, result)
} // namespace logic_agent::types::logical_consequence

#endif // ELOQUENTLOGIC_AGENT_LOGICAL_CONSEQUENCE_EVENTS_HPP
