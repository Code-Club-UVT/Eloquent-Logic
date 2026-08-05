//
// Created by vscode on 7/30/26.
//

#include "lexer.hpp"
#include <functional>
#include <map>
#include <sstream>
#include "dictionary.h"
namespace eloquent::logic
{
    enum class LexMode
    {
        None, // default
        AtomName, // P
        AtomSubscriptChar, // _
        AtomOpenMulticharSubscript, // {
        AtomMulticharSubscript,// {123425..
        AtomMulticharZeroStart, // {0
        Operator, // \..
        Error // error
    };
    struct lexer_data
    {
        std::vector<lexeme> lexemes;
        std::ostringstream buffer;
        bool lexemeFlag = false;
        lexeme_type type;
    };

    inline bool is_final(LexMode mode)
    {
        return mode == LexMode::None;
    }
    using transition_func = std::function<LexMode(const char&, lexer_data&)> ;
    LexMode none_func(const char& c, lexer_data& ld)
    {
        if (c == 3) //ETX - do nothing
            return LexMode::None;
        if (isalpha(c))
        {
            ld.buffer<<c;
            return LexMode::AtomName;
        }
        if (c == '(')
        {
            ld.buffer<<c;
            ld.lexemeFlag = true;
            ld.type = lexeme_type::LParen;
            return LexMode::None;
        }
        if (c == ')')
        {
            ld.buffer<<c;
            ld.lexemeFlag = true;
            ld.type = lexeme_type::RParen;
            return LexMode::None;
        }
        if (c == '\\')
        {
            ld.buffer<<c;
            return LexMode::Operator;
        }
        return LexMode::Error;
    }
    LexMode atom_name_func(const char& c, lexer_data& ld)
    {
        if (c == 3) // one atom left
        {
            ld.lexemeFlag = true;
            ld.type = lexeme_type::Atom;
            return LexMode::None;
        }
        if (c == '_') // subscript
        {
            ld.buffer<<c;
            return LexMode::AtomSubscriptChar;
        }
        if (c == '\\')
        {
            ld.lexemeFlag = true;
            ld.type = lexeme_type::Atom;
            return LexMode::Operator;
        }
        return LexMode::Error;
    }

    LexMode atom_multichar_zero_start_func(const char& c, lexer_data& ld)
    {
        if (c == '0') //P_{0..0} <=> P_0
            return LexMode::AtomMulticharZeroStart;
        if (isdigit(c))
        {
            ld.buffer<<c;
            return LexMode::AtomMulticharSubscript;
        }
        if (c == '}')//P_{0}
        {
            ld.lexemeFlag = true;
            ld.type = lexeme_type::Atom;
            return LexMode::None;
        }
        return LexMode::Error;
    }
    LexMode atom_subscript_func(const char& c, lexer_data& ld)
    {
        if (isdigit(c))
        {
            ld.buffer<<c;
            ld.lexemeFlag = true;
            ld.type = lexeme_type::Atom;
            return LexMode::None;
        }
        if (c == '{')
        {
            ld.buffer<<c;
            return LexMode::AtomOpenMulticharSubscript;
        }
        return LexMode::Error;
    }

    LexMode atom_open_multichar_subscript_func(const char& c, lexer_data& ld)
    {
        if (isdigit(c))
        {
            ld.buffer<<c;
            if (c=='0')
                return LexMode::AtomMulticharZeroStart;
            return LexMode::AtomMulticharSubscript;
        }
        return LexMode::Error;
    }
    LexMode atom_multichar_subscript_func(const char& c, lexer_data& ld)
    {
        if (isdigit(c))
        {
            ld.buffer<<c;
            return LexMode::AtomOpenMulticharSubscript;
        }
        if (c == '}')
        {
            ld.buffer<<c;
            ld.lexemeFlag = true;
            ld.type = lexeme_type::Atom;
            return LexMode::None;
        }
        return LexMode::Error;
    }
    LexMode operator_func(const char& c, lexer_data& ld)
    {
        if (c != 3) // 3 - ETX
            ld.buffer<<c;
        if (ld.buffer.str() == symbols::SYMB_IFF)
        {
            ld.lexemeFlag = true;
            ld.type = lexeme_type::IffOp;
            return LexMode::None;
        }
        if (ld.buffer.str() == symbols::SYMB_TAUTOLOGY)
        {
            ld.lexemeFlag = true;
            ld.type = lexeme_type::Atom;
            return LexMode::None;
        }
        if (ld.buffer.str() == symbols::SYMB_CONTRADICTION)
        {
            ld.lexemeFlag = true;
            ld.type = lexeme_type::Atom;
            return LexMode::None;
        }
        if (ld.buffer.str() == symbols::SYMB_AND)
        {
            ld.lexemeFlag = true;
            ld.type = lexeme_type::AndOp;
            return LexMode::None;
        }
        if (ld.buffer.str() == symbols::SYMB_OR)
        {
            ld.lexemeFlag = true;
            ld.type = lexeme_type::OrOp;
            return LexMode::None;
        }
        if (ld.buffer.str() == symbols::SYMB_IMPL)
        {
            ld.lexemeFlag = true;
            ld.type = lexeme_type::ImpliesOp;
            return LexMode::None;
        }
        if (ld.buffer.str() == symbols::SYMB_LEQUI)
        {
            ld.lexemeFlag = true;
            ld.type = lexeme_type::LEquiOp;
            return LexMode::None;
        }
        if (c != 3) // not end of text
            return LexMode::Operator;
        return LexMode::Error;
    }
    const std::map<LexMode,transition_func> transition_table = {
        {LexMode::None, none_func},
        {LexMode::AtomName, atom_name_func},
        {LexMode::AtomOpenMulticharSubscript, atom_open_multichar_subscript_func},
        {LexMode::AtomMulticharSubscript, atom_multichar_subscript_func},
        {LexMode::AtomSubscriptChar, atom_subscript_func},
        {LexMode::AtomMulticharZeroStart, atom_subscript_func},
        {LexMode::Operator, operator_func},
    };
    //TODO: better diagnostics
    std::vector<lexeme> lexer::lex(std::string_view text, const std::shared_ptr<lexer_listener_t>& listener)
    {
        lexer_data ld;
        LexMode mode = LexMode::None;
        listener->didStart();
        std::vector<lexeme> lexemes;
        if (text.empty())
        {
            listener->didReachEof();
            lexemes.emplace_back(lexeme::make(lexeme_type::Eof, "",0,0));
            listener->didFinish();
            return lexemes;
        }
        size_t last_change = 0;
        bool in_lexeme = false;
        for (size_t i = 0; i < text.size(); ++i)
        {
            listener->didReadCharacter(text[i]);
            if (isspace(text[i])) continue;
            LexMode new_mode = transition_table.at(mode)(text[i], ld);
            if (!in_lexeme)
            {
                last_change = i;
                in_lexeme = true;
            }
            if (new_mode == LexMode::Error)
            {
                auto sequence = text.substr(last_change);
                listener->didFindUnexpectedSequence(sequence,last_change);
                listener->didFinish();
                throw lexer_exception(sequence, last_change);
            }
            if (ld.lexemeFlag)
            {
                lexeme l = lexeme::make(ld.type, ld.buffer.str(), last_change, i);
                lexemes.emplace_back(l);
                listener->didRecogniseLexeme(l);
                ld.lexemeFlag = false;
                ld.type = lexeme_type::Unknown;
                ld.buffer.str("");
                in_lexeme = false;

            }

        }
        {
            LexMode new_mode = transition_table.at(mode)(3, ld); // ETX
            if (new_mode == LexMode::Error)
            {
                auto sequence = text.substr(last_change);
                listener->didFindUnexpectedSequence(sequence,last_change);
                listener->didFinish();
                throw lexer_exception(sequence, last_change);
            }
        }
        if (ld.lexemeFlag)
        {
            lexeme l = lexeme::make(ld.type, ld.buffer.str(), last_change,text.size()-1);
            lexemes.emplace_back(l);
            listener->didRecogniseLexeme(l);
            ld.lexemeFlag = false;
            ld.type = lexeme_type::Unknown;
            ld.buffer.str("");
        }
        listener->didReachEof();
        lexemes.emplace_back(lexeme::make(lexeme_type::Eof, "",text.size(), text.size()));
        return lexemes;
    }




} // logic
// eloquent