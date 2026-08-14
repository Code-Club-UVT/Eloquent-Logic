#ifndef ELOQUENTLOGIC_AGENT_DISPATCHER_HPP
#define ELOQUENTLOGIC_AGENT_DISPATCHER_HPP

#include <nlohmann/json.hpp>

#include "../types/rpc_request.hpp"
#include "event_sink.hpp"

// Looks up request.method in the method registry (truth_table, parse,
// transform, sat, shutdown) and runs it, streaming steps through `sink` as
// it goes and returning the JSON-RPC "result" payload alongside whether the
// server should exit its request loop after sending that result (true only
// for "shutdown" — see agent.cpp). Throws
// rpc_exception(error_code::method_not_found, ...) for an unknown method;
// anything a handler itself throws (rpc_exception or a liblogic exception)
// simply propagates to the caller.
namespace logic_agent::rpc
{
    struct dispatch_result
    {
        nlohmann::json result;
        bool should_shutdown = false;
    };

    dispatch_result dispatch(const rpc_request& request, event_sink& sink);
}

#endif //ELOQUENTLOGIC_AGENT_DISPATCHER_HPP
