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
        return lexemes[idx];
    }

    lexeme lexeme_stream::peek()
    {
        if (lexemes.empty())
            return lexeme::make(lexeme_type::Eof,"",0,0);
        if (idx+1 >= lexemes.size())
            return lexemes[idx];
        return lexemes[idx+1];
    }

    lexeme lexeme_stream::next()
    {
        if (lexemes.empty())
            return lexeme::make(lexeme_type::Eof,"",0,0);
        if (idx+1 > lexemes.size())
            return lexemes[idx];
        return lexemes[++idx];
    }
}