#ifndef ELOQUENTLOGIC_AGENT_SHUTDOWN_METHOD_HPP
#define ELOQUENTLOGIC_AGENT_SHUTDOWN_METHOD_HPP

#include <nlohmann/json.hpp>

#include "../event_sink.hpp"

// RPC method "shutdown": {} -> {message: string}
// Takes no params and streams no notifications (there's no formula pipeline
// to report steps from). The server sends this method's Response as normal;
// dispatcher.cpp's registry then tells agent.cpp's main loop to exit after
// that Response is flushed — no further Requests are read from STDIN.
namespace logic_agent::rpc::methods {
nlohmann::json handle_shutdown(const nlohmann::json &params, event_sink &sink);
}

#endif // ELOQUENTLOGIC_AGENT_SHUTDOWN_METHOD_HPP
