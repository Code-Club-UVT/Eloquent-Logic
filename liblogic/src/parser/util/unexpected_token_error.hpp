//
// Created by vscode on 7/31/26.
//

#ifndef ELOQUENTLOGIC_UNEXPECTED_TOKEN_ERROR_HPP
#define ELOQUENTLOGIC_UNEXPECTED_TOKEN_ERROR_HPP
#include "lexeme.hpp"
#include <exception>
#include <format>
namespace eloquent::logic
{
    class unexpected_token_error : public std::exception
    {
        lexeme m_lexeme;
        public:
            explicit unexpected_token_error(lexeme lexeme) : m_lexeme(lexeme) {}
        const char* what() const noexcept
        {
            return fmt::format("Unexpected token {} at {}\n", m_lexeme.token(), m_lexeme.position() ).c_str();
        }
    };
}


#endif //ELOQUENTLOGIC_UNEXPECTED_TOKEN_ERROR_HPP
