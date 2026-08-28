//
// Created by vscode on 7/20/26.
//

#ifndef ELOQUENTLOGIC_LEXEME_HPP
#define ELOQUENTLOGIC_LEXEME_HPP

#include <string>
#include <utility>

#include "hash_combine.hpp"

namespace eloquent::logic {

enum class lexeme_type {
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
    Eof,
    Unknown // blank
};

bool is_nary_operator(lexeme_type node_type);
bool is_operator(lexeme_type node_type);
bool is_atom(lexeme_type node_type);

class lexeme {
    lexeme_type m_node_type;
    std::string m_token;
    size_t m_start;
    size_t m_end;
    lexeme(lexeme_type node_type, std::string token, size_t start, size_t end)
        : m_node_type(node_type), m_token(std::move(token)), m_start(start),
          m_end(end) {}

  public:
    lexeme(const lexeme &l) = default;
    lexeme &operator=(const lexeme &l) = default;
    bool operator==(const lexeme &other) const = default;
    static lexeme make(lexeme_type node_type, std::string token, size_t start,
                       size_t end);
    [[nodiscard]] lexeme_type type() const;
    [[nodiscard]] std::string token() const;
    [[nodiscard]] size_t start() const;
    [[nodiscard]] size_t end() const;
};

} // namespace eloquent::logic

namespace std {
template <> struct hash<eloquent::logic::lexeme> {
    size_t operator()(const eloquent::logic::lexeme &x) const noexcept {
        size_t seed;
        eloquent::logic::hash_combine(seed, x.type());
        eloquent::logic::hash_combine(seed, x.token());
        eloquent::logic::hash_combine(seed, x.start());
        eloquent::logic::hash_combine(seed, x.end());
        return seed;
    }
};
} // namespace std

#endif // ELOQUENTLOGIC_LEXEME_HPP
