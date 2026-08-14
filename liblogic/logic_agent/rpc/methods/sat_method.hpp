#ifndef ELOQUENTLOGIC_AGENT_SAT_METHOD_HPP
#define ELOQUENTLOGIC_AGENT_SAT_METHOD_HPP

#include <nlohmann/json.hpp>

#include "../event_sink.hpp"

// RPC method "sat": {formula, strict?, algorithm?: "dp"|"dpll_mcl"|"dpll_rand"|"resolution" (default "dpll_mcl")}
//   -> {state: "SAT"|"UNSAT"|"UNKNOWN", mapping: {var: literal}, clauses: [[literal...]...]}
// Always converts the parsed formula to CNF first (SAT solving requires it),
// regardless of what CNF conversion produces being independently reachable
// via the "transform" method.
namespace logic_agent::rpc::methods
{
    nlohmann::json handle_sat(const nlohmann::json& params, event_sink& sink);
}

#endif //ELOQUENTLOGIC_AGENT_SAT_METHOD_HPP
