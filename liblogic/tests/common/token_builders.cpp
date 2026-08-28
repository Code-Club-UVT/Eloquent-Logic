#include "token_builders.hpp"

namespace eloquent::logic::testing {
lexeme Atom(const std::string &name) {
    return lexeme::make(lexeme_type::Atom, name, 0, 0);
}

lexeme Sym(lexeme_type type, const std::string &val) {
    return lexeme::make(type, val, 0, 0);
}

std::vector<lexeme> make_stream(const std::vector<lexeme> &tokens) {
    std::vector<lexeme> stream = tokens;
    stream.push_back(lexeme::make(lexeme_type::Eof, "", 0, 0));
    return stream;
}
} // namespace eloquent::logic::testing
