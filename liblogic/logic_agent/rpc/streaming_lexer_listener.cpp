#include "streaming_lexer_listener.hpp"

#include <string>

#include "../types/core/convert.hpp"
#include "../types/parser/lexer_events.hpp"

namespace logic_agent::rpc {
void streaming_lexer_listener::didStart() { sink.emit_empty("lexer/didStart"); }

void streaming_lexer_listener::didFinish() {
    sink.emit_empty("lexer/didFinish");
}

void streaming_lexer_listener::didRecogniseLexeme(
    const eloquent::logic::lexeme &l) {
    logic_agent::types::parser::single_lexeme_event_t dto;
    dto.lexeme = logic_agent::types::to_dto(l);
    sink.emit("lexer/didRecogniseLexeme", dto);
}

void streaming_lexer_listener::didReadCharacter(const char &c) {
    logic_agent::types::parser::character_event_t dto;
    dto.value = std::string(1, c);
    sink.emit("lexer/didReadCharacter", dto);
}

void streaming_lexer_listener::didFindUnrecognisedOperator(
    std::string_view text, size_t position) {
    logic_agent::types::parser::unrecognised_operator_event_t dto;
    dto.operator_text = std::string(text);
    dto.position = position;
    sink.emit("lexer/didFindUnrecognisedOperator", dto);
}

void streaming_lexer_listener::didFindMissingSubscript(size_t position) {
    logic_agent::types::parser::subscript_position_event_t dto;
    dto.position = position;
    sink.emit("lexer/didFindMissingSubscript", dto);
}

void streaming_lexer_listener::didFindUnbalancedSubscript(size_t position) {
    logic_agent::types::parser::subscript_position_event_t dto;
    dto.position = position;
    sink.emit("lexer/didFindUnbalancedSubscript", dto);
}

void streaming_lexer_listener::didFindUnexpectedSequence(
    std::string_view sequence, size_t position) {
    logic_agent::types::parser::unexpected_sequence_event_t dto;
    dto.sequence = std::string(sequence);
    dto.position = position;
    sink.emit("lexer/didFindUnexpectedSequence", dto);
}

void streaming_lexer_listener::didReachEof() {
    sink.emit_empty("lexer/didReachEof");
}
} // namespace logic_agent::rpc
