#include "streaming_logical_consequence_listener.hpp"

#include "../types/logical_consequence/logical_consequence_events.hpp"

namespace logic_agent::rpc {
void streaming_logical_consequence_listener::didStart() {
    sink.emit_empty("logical_consequence/didStart");
}

void streaming_logical_consequence_listener::didFinish() {
    sink.emit_empty("logical_consequence/didFinish");
}

void streaming_logical_consequence_listener::not_logical_equivalence() {
    sink.emit_empty("logical_consequence/not_logical_equivalence");
}

void streaming_logical_consequence_listener::finalised_with_result(
    bool result) {
    logic_agent::types::logical_consequence::result_event_t dto;
    dto.result = result;
    sink.emit("logical_consequence/finalised_with_result", dto);
}
} // namespace logic_agent::rpc
