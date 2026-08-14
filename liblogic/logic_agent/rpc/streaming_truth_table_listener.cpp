#include "streaming_truth_table_listener.hpp"

#include <string>

#include "../types/core/convert.hpp"
#include "../types/truth_table/truth_table_events.hpp"

namespace logic_agent::rpc
{
    void streaming_truth_table_listener::didStart() { sink.emit_empty("truth_table/didStart"); }

    void streaming_truth_table_listener::didFinish() { sink.emit_empty("truth_table/didFinish"); }

    void streaming_truth_table_listener::didFindUnknownVariable(const std::string_view& name)
    {
        logic_agent::types::truth_table::unknown_variable_event_t dto;
        dto.name = std::string(name);
        sink.emit("truth_table/didFindUnknownVariable", dto);
    }

    void streaming_truth_table_listener::didSetVariable(const std::string_view& name, bool value)
    {
        logic_agent::types::truth_table::variable_assignment_event_t dto;
        dto.name = std::string(name);
        dto.value = value;
        sink.emit("truth_table/didSetVariable", dto);
    }

    void streaming_truth_table_listener::didComputeSubexpression(const CppCommon::UUID& node_id, bool value)
    {
        logic_agent::types::truth_table::subexpression_result_event_t dto;
        dto.node_id = logic_agent::types::to_dto(node_id);
        dto.value = value;
        sink.emit("truth_table/didComputeSubexpression", dto);
    }
}
