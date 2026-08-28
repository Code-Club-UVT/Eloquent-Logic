#ifndef ELOQUENTLOGIC_AGENT_LEXEME_T_HPP
#define ELOQUENTLOGIC_AGENT_LEXEME_T_HPP

#include <cstddef>
#include <string>

#include <nlohmann/json.hpp>

namespace logic_agent::types {
// Mirrors eloquent::logic::lexeme_type (src/parser/lexeme.hpp).
// `Unknown` is listed first so it is the fallback value nlohmann's
// enum macro uses when deserializing an unrecognised string.
enum class lexeme_type_t {
    Unknown,
    Atom,
    Tautology,
    Contradiction,
    LParen,
    RParen,
    NotOp,
    AndOp,
    OrOp,
    ImpliesOp,
    IffOp,
    LEquiOp,
    Eof
};

NLOHMANN_JSON_SERIALIZE_ENUM(lexeme_type_t,
                             {
                                 {lexeme_type_t::Unknown, "Unknown"},
                                 {lexeme_type_t::Atom, "Atom"},
                                 {lexeme_type_t::Tautology, "Tautology"},
                                 {lexeme_type_t::Contradiction,
                                  "Contradiction"},
                                 {lexeme_type_t::LParen, "LParen"},
                                 {lexeme_type_t::RParen, "RParen"},
                                 {lexeme_type_t::NotOp, "NotOp"},
                                 {lexeme_type_t::AndOp, "AndOp"},
                                 {lexeme_type_t::OrOp, "OrOp"},
                                 {lexeme_type_t::ImpliesOp, "ImpliesOp"},
                                 {lexeme_type_t::IffOp, "IffOp"},
                                 {lexeme_type_t::LEquiOp, "LEquiOp"},
                                 {lexeme_type_t::Eof, "Eof"},
                             })

// Mirrors eloquent::logic::lexeme (src/parser/lexeme.hpp).
struct lexeme_t {
    lexeme_type_t type = lexeme_type_t::Unknown;
    std::string token;
    size_t start = 0;
    size_t end = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(lexeme_t, type, token, start, end)
} // namespace logic_agent::types

#endif // ELOQUENTLOGIC_AGENT_LEXEME_T_HPP
