#ifndef ELOQUENTLOGIC_AGENT_RELAXED_PARSER_EVENTS_HPP
#define ELOQUENTLOGIC_AGENT_RELAXED_PARSER_EVENTS_HPP

#include <optional>

#include <nlohmann/json.hpp>

#include "../core/lexeme_t.hpp"
#include "../core/node_t.hpp"
#include "../feedback_base.hpp"
#include "lexer_events.hpp"

// DTOs for eloquent::logic::relaxed_parser_listener_t
// (src/parser/relaxed_parser_listener.hpp) callback parameters that aren't
// already covered by a core/cursor type:
//   - foundUnexpectedToken(lexeme) / mismatchedParens(lexeme) /
//     startParsingParens(lexeme) / didFindLowerPrecendenceOperator(lexeme) /
//     didReadLexeme(lexeme)
//       -> logic_agent::types::parser::single_lexeme_event_t (lexer_events.hpp)
//   - didFindPureAtomInParens(lexeme, lexeme, lexeme) ->
//   pure_atom_in_parens_event_t
//   - didMakeNewSubtree(Node*)                        ->
//   logic_agent::types::parser::single_node_event_t (cursor_events.hpp)
//   - didJoin(Node* target, Node* source)             -> join_event_t
//   - didParseExpression() / parenthesisClosedTooEarly() -> no payload
namespace logic_agent::types::parser {
// didFindPureAtomInParens(open, atom, close) — confirmed via
// relaxed_parser.cpp: stream.current() is the just-seen "(",
// peek(1)/peek(2) are the atom and the closing ")".
struct pure_atom_in_parens_event_t : public feedback_base {
    lexeme_t open_paren;
    lexeme_t atom;
    lexeme_t close_paren;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(pure_atom_in_parens_event_t, open_paren,
                                   atom, close_paren)

// didJoin(Node* target, Node* source) — target is the node being grown,
// source is the subtree being merged into it (see relaxed_parser.cpp
// call sites).
struct join_event_t : public feedback_base {
    std::optional<node_t> target;
    std::optional<node_t> source;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(join_event_t, target, source)
} // namespace logic_agent::types::parser

#endif // ELOQUENTLOGIC_AGENT_RELAXED_PARSER_EVENTS_HPP
