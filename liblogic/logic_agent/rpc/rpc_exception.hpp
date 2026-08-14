#ifndef ELOQUENTLOGIC_AGENT_RPC_EXCEPTION_HPP
#define ELOQUENTLOGIC_AGENT_RPC_EXCEPTION_HPP

#include <stdexcept>
#include <string>

// Thrown by RPC-layer code (request parsing, method dispatch, param
// validation) to carry a JSON-RPC 2.0 error code alongside the message.
// Anything else derived from std::exception that escapes a method handler
// (liblogic's own lexer_exception/unknown_variable_error/unbound_variable_error/
// unsupported_operator_error/etc.) is caught generically in agent.cpp and
// reported under error_code::server_error instead.
namespace logic_agent::rpc
{
    // Standard JSON-RPC 2.0 error codes:
    // https://www.jsonrpc.org/specification#error_object
    namespace error_code
    {
        constexpr int parse_error = -32700;
        constexpr int invalid_request = -32600;
        constexpr int method_not_found = -32601;
        constexpr int invalid_params = -32602;
        constexpr int internal_error = -32603;
        constexpr int server_error = -32000;
    }

    class rpc_exception : public std::runtime_error
    {
        int m_code;

    public:
        rpc_exception(int code, const std::string& message) : std::runtime_error(message), m_code(code) {}

        [[nodiscard]] int code() const noexcept { return m_code; }
    };
}

#endif //ELOQUENTLOGIC_AGENT_RPC_EXCEPTION_HPP
