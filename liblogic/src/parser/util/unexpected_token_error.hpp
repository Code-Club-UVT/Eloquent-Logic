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
    class unexpected_token_error : public std::exception
    {
        lexeme m_lexeme;
        public:
            explicit unexpected_token_error(lexeme lexeme) : m_lexeme(lexeme) {}
        [[nodiscard]] const char* what() const noexcept final;
    };
}


#endif //ELOQUENTLOGIC_UNEXPECTED_TOKEN_ERROR_HPP
