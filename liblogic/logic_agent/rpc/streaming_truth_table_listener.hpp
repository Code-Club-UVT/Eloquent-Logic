#ifndef ELOQUENTLOGIC_AGENT_STREAMING_TRUTH_TABLE_LISTENER_HPP
#define ELOQUENTLOGIC_AGENT_STREAMING_TRUTH_TABLE_LISTENER_HPP

#include <string_view>

#include <system/uuid.h>
#include <truth_table_listener_t.hpp>

#include "event_sink.hpp"

// Streams every eloquent::logic::truth_table_listener_t callback as a
// "truth_table/<callbackName>" JSON-RPC notification via `sink`.
namespace logic_agent::rpc {
class streaming_truth_table_listener
    : public eloquent::logic::truth_table_listener_t {
    event_sink &sink;

  public:
    explicit streaming_truth_table_listener(event_sink &sink) : sink(sink) {}

    void didStart() override;
    void didFinish() override;
    void didFindUnknownVariable(const std::string_view &name) override;
    void didSetVariable(const std::string_view &name, bool value) override;
    void didComputeSubexpression(const CppCommon::UUID &node_id,
                                 bool value) override;
};
} // namespace logic_agent::rpc

#endif // ELOQUENTLOGIC_AGENT_STREAMING_TRUTH_TABLE_LISTENER_HPP
