//
// Shared lexeme/token-stream builders for tests.
//
#pragma once

#include <string>
#include <vector>

#include <lexeme.hpp>

namespace eloquent::logic::testing
{
    lexeme Atom(const std::string& name);

    lexeme Sym(lexeme_type type, const std::string& val = "");

    std::vector<lexeme> make_stream(const std::vector<lexeme>& tokens);
}
