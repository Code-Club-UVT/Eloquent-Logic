#ifndef ELOQUENTLOGIC_AGENT_LEXER_EVENTS_HPP
#define ELOQUENTLOGIC_AGENT_LEXER_EVENTS_HPP

#include <cstddef>
#include <string>

#include <nlohmann/json.hpp>

// DTOs for eloquent::logic::lexer_listener_t (src/parser/lexer_listener_t.hpp)
// callback parameters that aren't already covered by a core type:
//   - didRecogniseLexeme(lexeme)              -> logic_agent::types::lexeme_t
//   - didReadCharacter(char)                  -> character_event_t
//   - didFindUnrecognisedOperator(string_view, size_t) -> unrecognised_operator_event_t
//   - didFindMissingSubscript(size_t)         -> subscript_position_event_t
//   - didFindUnbalancedSubscript(size_t)      -> subscript_position_event_t
//   - didFindUnexpectedSequence(string_view, size_t)   -> unexpected_sequence_event_t
//   - didReachEof() / didStart() / didFinish() -> no payload
namespace logic_agent::types::parser
{
    // didReadCharacter — stored as a 1-character string rather than a raw
    // JSON number so the payload reads as text.
    struct character_event_t
    {
        std::string value;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(character_event_t, value);

    struct unrecognised_operator_event_t
    {
        std::string operator_text;
        size_t position = 0;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(unrecognised_operator_event_t, operator_text, position);

    // Shared by didFindMissingSubscript and didFindUnbalancedSubscript,
    // which both only carry a position.
    struct subscript_position_event_t
    {
        size_t position = 0;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(subscript_position_event_t, position);

    struct unexpected_sequence_event_t
    {
        std::string sequence;
        size_t position = 0;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(unexpected_sequence_event_t, sequence, position);
}

#endif //ELOQUENTLOGIC_AGENT_LEXER_EVENTS_HPP
