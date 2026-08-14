//
// Created by vscode on 7/31/26.
//

#ifndef ELOQUENTLOGIC_UNEXPECTED_TOKEN_ERROR_HPP
#define ELOQUENTLOGIC_UNEXPECTED_TOKEN_ERROR_HPP
#include "lexeme.hpp"
#include <exception>
#include <format>
#include <string/format.h>

namespace eloquent::logic
{
    class unknown_variable_error : public std::exception
    {
        lexeme m_lexeme;
        std::string message;
        public:
            explicit unknown_variable_error(lexeme lexeme) : m_lexeme(lexeme), message(fmt::format("Unexpected token {} at [{},{}]\n", m_lexeme.token(), m_lexeme.start(), m_lexeme.end() )) {}
        [[nodiscard]] const char* what() const noexcept final;
    };
}


#endif //ELOQUENTLOGIC_UNEXPECTED_TOKEN_ERROR_HPP
