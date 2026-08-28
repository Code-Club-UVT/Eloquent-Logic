#ifndef ELOQUENTLOGIC_AGENT_PARSE_METHOD_HPP
#define ELOQUENTLOGIC_AGENT_PARSE_METHOD_HPP

#include <nlohmann/json.hpp>

#include "../event_sink.hpp"

// RPC method "parse": {formula, strict?} -> {tree: <node_t>}
namespace logic_agent::rpc::methods {
nlohmann::json handle_parse(const nlohmann::json &params, event_sink &sink);
}

#endif // ELOQUENTLOGIC_AGENT_PARSE_METHOD_HPP
