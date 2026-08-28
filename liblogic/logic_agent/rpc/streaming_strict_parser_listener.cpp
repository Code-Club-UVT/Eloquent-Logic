#include "streaming_strict_parser_listener.hpp"

#include "../types/core/convert.hpp"
#include "../types/parser/cursor_events.hpp"
#include "../types/parser/lexer_events.hpp"
#include "../types/parser/strict_parser_events.hpp"

namespace logic_agent::rpc {
void streaming_strict_parser_listener::didStart() {
    sink.emit_empty("parser/strict/didStart");
}

void streaming_strict_parser_listener::didFinish() {
    sink.emit_empty("parser/strict/didFinish");
}

void streaming_strict_parser_listener::didGoUp() {
    sink.emit_empty("parser/strict/didGoUp");
}

void streaming_strict_parser_listener::didTryInvalidPosition(
    eloquent::logic::NodeObsPtr node, size_t idx) {
    logic_agent::types::parser::invalid_position_event_t dto;
    dto.node = logic_agent::types::to_dto(node);
    dto.index = idx;
    sink.emit("parser/strict/didTryInvalidPosition", dto);
}

void streaming_strict_parser_listener::didTrySpawiningOverExistingNode(
    eloquent::logic::NodeObsPtr node) {
    logic_agent::types::parser::single_node_event_t dto;
    dto.node = logic_agent::types::to_dto(node);
    sink.emit("parser/strict/didTrySpawiningOverExistingNode", dto);
}

void streaming_strict_parser_listener::spawnedNewChildNode(
    const eloquent::logic::NodeObsPtr &parent,
    const eloquent::logic::NodeObsPtr &child) {
    logic_agent::types::parser::spawned_child_event_t dto;
    dto.parent = logic_agent::types::to_dto(parent);
    dto.child = logic_agent::types::to_dto(child);
    sink.emit("parser/strict/spawnedNewChildNode", dto);
}

void streaming_strict_parser_listener::didPlaceNode() {
    sink.emit_empty("parser/strict/didPlaceNode");
}

void streaming_strict_parser_listener::alreadyUp() {
    sink.emit_empty("parser/strict/alreadyUp");
}

void streaming_strict_parser_listener::didProcessLexeme(
    const eloquent::logic::lexeme &l) {
    logic_agent::types::parser::single_lexeme_event_t dto;
    dto.lexeme = logic_agent::types::to_dto(l);
    sink.emit("parser/strict/didProcessLexeme", dto);
}

void streaming_strict_parser_listener::didMakeNode(
    const eloquent::logic::NodeObsPtr &node) {
    logic_agent::types::parser::single_node_event_t dto;
    dto.node = logic_agent::types::to_dto(node);
    sink.emit("parser/strict/didMakeNode", dto);
}

void streaming_strict_parser_listener::didPlaceNodeAtCursor(
    const eloquent::logic::NodeObsPtr &node) {
    logic_agent::types::parser::single_node_event_t dto;
    dto.node = logic_agent::types::to_dto(node);
    sink.emit("parser/strict/didPlaceNodeAtCursor", dto);
}

void streaming_strict_parser_listener::foundUnexpectedToken(
    const eloquent::logic::lexeme &l) {
    logic_agent::types::parser::single_lexeme_event_t dto;
    dto.lexeme = logic_agent::types::to_dto(l);
    sink.emit("parser/strict/foundUnexpectedToken", dto);
}

void streaming_strict_parser_listener::foundMismatchedParanthesis() {
    sink.emit_empty("parser/strict/foundMismatchedParanthesis");
}

void streaming_strict_parser_listener::startedProcessingParanthesis() {
    sink.emit_empty("parser/strict/startedProcessingParanthesis");
}

void streaming_strict_parser_listener::foundAstError(
    const eloquent::logic::Cursor &cursor) {
    logic_agent::types::parser::ast_error_event_t dto;
    dto.cursor = logic_agent::types::to_dto(cursor);
    sink.emit("parser/strict/foundAstError", dto);
}

void streaming_strict_parser_listener::acceptUnaryOpVariant() {
    sink.emit_empty("parser/strict/acceptUnaryOpVariant");
}

void streaming_strict_parser_listener::acceptBinaryOpVariant() {
    sink.emit_empty("parser/strict/acceptBinaryOpVariant");
}

void streaming_strict_parser_listener::wrongArityForNode(
    eloquent::logic::NodeObsPtr node) {
    logic_agent::types::parser::single_node_event_t dto;
    dto.node = logic_agent::types::to_dto(node);
    sink.emit("parser/strict/wrongArityForNode", dto);
}
} // namespace logic_agent::rpc
