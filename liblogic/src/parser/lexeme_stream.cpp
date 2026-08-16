//
// Created by vscode on 8/1/26.
//

#include "lexeme_stream.hpp"
namespace eloquent::logic
{
    bool lexeme_stream::can_continue() const noexcept
    {
        return idx < lexemes.size();
    }

    lexeme_stream::lexeme_stream(std::vector<lexeme> lexemes): lexemes(std::move(lexemes)){}

    lexeme lexeme_stream::current()
    {
        if (lexemes.empty())
            return lexeme::make(lexeme_type::Eof,"",0,0);
        if (idx >= lexemes.size())
            return lexemes.back();
        return lexemes[idx];
    }

    lexeme lexeme_stream::peek(const size_t offset)
    {
        if (lexemes.empty())
            return lexeme::make(lexeme_type::Eof,"",0,0);
        if (idx + offset >= lexemes.size())
            return lexemes.back();
        return lexemes[idx+offset];
    }

    lexeme lexeme_stream::next()
    {
        if (lexemes.empty())
            return lexeme::make(lexeme_type::Eof,"",0,0);
        if (idx < lexemes.size())
            ++idx;
        if (idx >= lexemes.size())
            return lexemes.back();
        return lexemes[idx];
    }
}