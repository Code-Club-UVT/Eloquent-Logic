//
// Created by vscode on 7/31/26.
//

#ifndef ELOQUENTLOGIC_UNKNOWN_VARIABLE_ERROR_HPP
#define ELOQUENTLOGIC_UNKNOWN_VARIABLE_ERROR_HPP
#include <exception>
#include <format>
#include <string/format.h>

namespace eloquent::logic
{
    class unknown_variable_error : public std::exception
    {
        std::string m_lexeme;
        std::string message;
        public:
            explicit unknown_variable_error(std::string lexeme) : m_lexeme(std::move(lexeme)), message(fmt::format("Unexpected variable {}\n", m_lexeme )) {}
        [[nodiscard]] const char* what() const noexcept final;
    };
}


#endif //ELOQUENTLOGIC_UNKNOWN_VARIABLE_ERROR_HPP
