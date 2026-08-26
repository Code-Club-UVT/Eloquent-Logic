#ifndef ELOQUENTLOGIC_AGENT_STREAMING_LOGICAL_CONSEQUENCE_LISTENER_HPP
#define ELOQUENTLOGIC_AGENT_STREAMING_LOGICAL_CONSEQUENCE_LISTENER_HPP

#include <logical_consequence_listener_t.hpp>

#include "event_sink.hpp"

// Streams every eloquent::logic::logical_consequence_listener_t callback as
// a "logical_consequence/<callbackName>" JSON-RPC notification via `sink`.
namespace logic_agent::rpc {
class streaming_logical_consequence_listener
    : public eloquent::logic::logical_consequence_listener_t {
    event_sink &sink;

  public:
    explicit streaming_logical_consequence_listener(event_sink &sink)
        : sink(sink) {}

    void didStart() override;
    void didFinish() override;

    void not_logical_equivalence() override;
    void finalised_with_result(bool result) override;
};
} // namespace logic_agent::rpc

#endif // ELOQUENTLOGIC_AGENT_STREAMING_LOGICAL_CONSEQUENCE_LISTENER_HPP
