//
// Created by vscode on 8/1/26.
//

#ifndef ELOQUENTLOGIC_LEXEME_STREAM_HPP
#define ELOQUENTLOGIC_LEXEME_STREAM_HPP
#include "lexeme.hpp"
#include <vector>
namespace eloquent::logic
{
    class lexeme_stream
    {
        size_t idx = 0;
        std::vector<lexeme> lexemes;
    public:
        [[nodiscard]] bool can_continue() const noexcept;
        explicit lexeme_stream(std::vector<lexeme> lexemes);
        [[nodiscard]] lexeme current();
        [[nodiscard]] lexeme peek();
        [[nodiscard]] lexeme next();
    };
}


#endif //ELOQUENTLOGIC_LEXEME_STREAM_HPP
