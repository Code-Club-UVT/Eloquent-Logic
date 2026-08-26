#include "streaming_relaxed_parser_listener.hpp"

#include "../types/core/convert.hpp"
#include "../types/parser/cursor_events.hpp"
#include "../types/parser/lexer_events.hpp"
#include "../types/parser/relaxed_parser_events.hpp"

namespace logic_agent::rpc {
void streaming_relaxed_parser_listener::didStart() {
    sink.emit_empty("parser/didStart");
}

void streaming_relaxed_parser_listener::didFinish() {
    sink.emit_empty("parser/didFinish");
}

void streaming_relaxed_parser_listener::didParseExpression() {
    sink.emit_empty("parser/didParseExpression");
}

void streaming_relaxed_parser_listener::foundUnexpectedToken(
    const eloquent::logic::lexeme &l) {
    logic_agent::types::parser::single_lexeme_event_t dto;
    dto.lexeme = logic_agent::types::to_dto(l);
    sink.emit("parser/foundUnexpectedToken", dto);
}

void streaming_relaxed_parser_listener::parenthesisClosedTooEarly() {
    sink.emit_empty("parser/parenthesisClosedTooEarly");
}

void streaming_relaxed_parser_listener::mismatchedParens(
    const eloquent::logic::lexeme &l) {
    logic_agent::types::parser::single_lexeme_event_t dto;
    dto.lexeme = logic_agent::types::to_dto(l);
    sink.emit("parser/mismatchedParens", dto);
}

void streaming_relaxed_parser_listener::didFindPureAtomInParens(
    const eloquent::logic::lexeme &open, const eloquent::logic::lexeme &atom,
    const eloquent::logic::lexeme &close) {
    logic_agent::types::parser::pure_atom_in_parens_event_t dto;
    dto.open_paren = logic_agent::types::to_dto(open);
    dto.atom = logic_agent::types::to_dto(atom);
    dto.close_paren = logic_agent::types::to_dto(close);
    sink.emit("parser/didFindPureAtomInParens", dto);
}

void streaming_relaxed_parser_listener::startParsingParens(
    const eloquent::logic::lexeme &l) {
    logic_agent::types::parser::single_lexeme_event_t dto;
    dto.lexeme = logic_agent::types::to_dto(l);
    sink.emit("parser/startParsingParens", dto);
}

void streaming_relaxed_parser_listener::didMakeNewSubtree(
    const eloquent::logic::NodeObsPtr &node) {
    logic_agent::types::parser::single_node_event_t dto;
    dto.node = logic_agent::types::to_dto(node);
    sink.emit("parser/didMakeNewSubtree", dto);
}

void streaming_relaxed_parser_listener::didJoin(
    const eloquent::logic::NodeObsPtr &target,
    const eloquent::logic::NodeObsPtr &source) {
    logic_agent::types::parser::join_event_t dto;
    dto.target = logic_agent::types::to_dto(target);
    dto.source = logic_agent::types::to_dto(source);
    sink.emit("parser/didJoin", dto);
}

void streaming_relaxed_parser_listener::didFindLowerPrecendenceOperator(
    const eloquent::logic::lexeme &l) {
    logic_agent::types::parser::single_lexeme_event_t dto;
    dto.lexeme = logic_agent::types::to_dto(l);
    sink.emit("parser/didFindLowerPrecendenceOperator", dto);
}

void streaming_relaxed_parser_listener::didReadLexeme(
    const eloquent::logic::lexeme &l) {
    logic_agent::types::parser::single_lexeme_event_t dto;
    dto.lexeme = logic_agent::types::to_dto(l);
    sink.emit("parser/didReadLexeme", dto);
}
} // namespace logic_agent::rpc
