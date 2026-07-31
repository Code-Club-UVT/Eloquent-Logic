//
// Created by vscode on 7/30/26.
//

#ifndef ELOQUENTLOGIC_LEXER_HPP
#define ELOQUENTLOGIC_LEXER_HPP

#include <exception>
#include <string>
#include <format>
#include <vector>
#include "lexeme.hpp"
#include "lexer_listener_t.hpp"

namespace eloquent::logic
{
    class lexer_exception : public std::exception
    {
        std::string sequence;
        size_t start_idx;
        public:
        constexpr lexer_exception(std::string_view msg, size_t start_idx) : sequence(std::string(msg)), start_idx(start_idx) {}
        [[nodiscard]] const char * what() const noexcept override
        {

            return fmt::format("Invalid sequence {} found starting at {}", this->sequence, this->start_idx).c_str();
        }
    };
    class lexer
    {
    public:

        static std::vector<lexeme> lex(std::string_view text, const std::shared_ptr<lexer_listener_t>& listener);

    };
} // logic
// eloquent

#endif //ELOQUENTLOGIC_LEXER_HPP
