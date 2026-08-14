#include "streaming_mapper_listener.hpp"

#include "../types/sat/convert.hpp"
#include "../types/sat/mapper_events.hpp"

namespace logic_agent::rpc
{
    void streaming_mapper_listener::didStart() { sink.emit_empty("mapper/didStart"); }

    void streaming_mapper_listener::didFinish() { sink.emit_empty("mapper/didFinish"); }

    void streaming_mapper_listener::did_map_literals(const std::map<std::string, Literal>& mapping)
    {
        logic_agent::types::sat::literal_mapping_event_t dto;
        dto.mapping = mapping; // Literal and logic_agent::types::sat::literal_t are both std::int64_t
        sink.emit("mapper/did_map_literals", dto);
    }

    void streaming_mapper_listener::did_map_clauses(const ClauseSet& clauses)
    {
        logic_agent::types::sat::clause_set_event_t dto;
        dto.clauses = logic_agent::types::sat::to_dto(clauses);
        sink.emit("mapper/did_map_clauses", dto);
    }
}
