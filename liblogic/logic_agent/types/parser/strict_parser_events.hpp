#ifndef ELOQUENTLOGIC_AGENT_STRICT_PARSER_EVENTS_HPP
#define ELOQUENTLOGIC_AGENT_STRICT_PARSER_EVENTS_HPP

#include <nlohmann/json.hpp>

#include "cursor_events.hpp"
#include "cursor_t.hpp"

// DTOs for eloquent::logic::strict_parser_listener_t
// (src/parser/strict_parser_listener_t.hpp) callback parameters that
// aren't already covered by a core/cursor type:
//   - didProcessLexeme(lexeme) / foundUnexpectedToken(lexeme) -> logic_agent::types::lexeme_t
//   - didMakeNode(Node*) / didPlaceNodeAtCursor(Node*) / wrongArityForNode(Node*)
//       -> logic_agent::types::parser::single_node_event_t (cursor_events.hpp)
//   - foundAstError(Cursor)                                   -> ast_error_event_t
//   - foundMismatchedParanthesis() / startedProcessingParanthesis() /
//     acceptUnaryOpVariant() / acceptBinaryOpVariant()         -> no payload
// strict_parser_listener_t also inherits cursor_listener_t's callbacks
// (didGoUp, didTryInvalidPosition, didTrySpawiningOverExistingNode,
// spawnedNewChildNode, didPlaceNode, alreadyUp) — see cursor_events.hpp.
namespace logic_agent::types::parser
{
    struct ast_error_event_t
    {
        cursor_t cursor;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ast_error_event_t, cursor)
}

#endif //ELOQUENTLOGIC_AGENT_STRICT_PARSER_EVENTS_HPP
