#ifndef ELOQUENTLOGIC_AGENT_LOGICAL_CONSEQUENCE_METHOD_HPP
#define ELOQUENTLOGIC_AGENT_LOGICAL_CONSEQUENCE_METHOD_HPP

#include <nlohmann/json.hpp>

#include "../event_sink.hpp"

// RPC method "logical_consequence": {formula, strict?}
//   -> {result: bool}
// Parses `formula` (which must have "\models" as its single top-level
// operator, splitting it into Premises \models Conclusion) and decides
// whether the entailment holds by checking Premises & ~Conclusion for
// unsatisfiability (eloquent::logic::logical_consequence::
// solve_logical_consequence, src/logical_consequence/logical_consequence.cpp).
// Throws incompatible_top_level_operator (reported as -32000 Server error,
// like every other liblogic exception) if `formula`'s top-level operator
// isn't "\models".
namespace logic_agent::rpc::methods {
nlohmann::json handle_logical_consequence(const nlohmann::json &params,
                                          event_sink &sink);
}

#endif // ELOQUENTLOGIC_AGENT_LOGICAL_CONSEQUENCE_METHOD_HPP
