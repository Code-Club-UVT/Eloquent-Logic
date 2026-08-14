#ifndef ELOQUENTLOGIC_AGENT_TRANSFORM_METHOD_HPP
#define ELOQUENTLOGIC_AGENT_TRANSFORM_METHOD_HPP

#include <nlohmann/json.hpp>

#include "../event_sink.hpp"

// RPC method "transform": {formula, strict?, target?: "nnf"|"cnf"|"dnf" (default "cnf")}
//   -> {tree: <node_t>}
namespace logic_agent::rpc::methods
{
    nlohmann::json handle_transform(const nlohmann::json& params, event_sink& sink);
}

#endif //ELOQUENTLOGIC_AGENT_TRANSFORM_METHOD_HPP
