#ifndef ELOQUENTLOGIC_AGENT_EVENT_SINK_HPP
#define ELOQUENTLOGIC_AGENT_EVENT_SINK_HPP

#include <concepts>
#include <ostream>
#include <string_view>
#include <utility>

#include <nlohmann/json.hpp>

#include "../types/feedback_base.hpp"

// Wraps the STDOUT stream the server streams "step" notifications to: one
// line per liblogic listener callback, each a JSON-RPC 2.0 Notification
// object (a Request object with no "id"):
//   {"jsonrpc":"2.0","method":"sat/didFindUnitLiteral","params":{"literal":3}}
// Every streaming_*_listener (logic_agent/rpc/streaming_*.hpp) holds a
// reference to one of these and calls emit()/emit_empty() from its callback
// overrides.
namespace logic_agent::rpc {
class event_sink {
    std::ostream &out;

    void write(std::string_view method, nlohmann::json params) const {
        nlohmann::json line{
            {"jsonrpc", "2.0"},
            {"method", method},
            {"params", std::move(params)},
        };
        out << line.dump() << "\n";
        out.flush();
    }

  public:
    explicit event_sink(std::ostream &out) : out(out) {}

    // Only accepts DTOs that are marked as streamable event payloads by
    // inheriting feedback_base (logic_agent/types/feedback_base.hpp; see
    // logic_agent/types/**/*_events.hpp) — a compile-time guard against
    // accidentally streaming an unrelated type.
    template <typename T>
        requires std::derived_from<T, feedback_base>
    void emit(std::string_view method, const T &payload) const {
        write(method, nlohmann::json(payload));
    }

    // For the many listener callbacks that intentionally have no DTO
    // (didGoUp, didConcludeSat, ...) because they carry no data.
    void emit_empty(std::string_view method) const {
        write(method, nlohmann::json::object());
    }
};
} // namespace logic_agent::rpc

#endif // ELOQUENTLOGIC_AGENT_EVENT_SINK_HPP
