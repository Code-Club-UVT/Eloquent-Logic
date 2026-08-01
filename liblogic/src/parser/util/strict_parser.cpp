//
// Created by vscode on 7/31/26.
//

#include "strict_parser.hpp"
#include "unexpected_token_error.hpp"
#include "cursor.h"

namespace eloquent::logic
{
    bool strict_parser::lexeme_stream::can_continue() const noexcept
    {
        return idx < lexemes.size();
    }

    strict_parser::lexeme_stream::lexeme_stream(std::vector<lexeme> lexemes): lexemes(std::move(lexemes)){}

    lexeme strict_parser::lexeme_stream::peek()
    {
        if (lexemes.empty())
            return lexeme::make(lexeme_type::Eof,"",0,0);
        if (idx+1 >= lexemes.size())
            return lexemes[idx];
        return lexemes[idx+1];
    }

    lexeme strict_parser::lexeme_stream::next()
    {
        if (lexemes.empty())
            return lexeme::make(lexeme_type::Eof,"",0,0);
        if (idx+1 >= lexemes.size())
            return lexemes[idx];
        return lexemes[++idx];
    }

    NodePtr strict_parser::parse_paranthesised_expression(Cursor& cursor, lexeme_stream& ls, const std::shared_ptr<parser_listener_t>& listener)
    {
        listener->startedProcessingParanthesis();
        bool has_content = false; // empty paranthesis MUST have something in them
        bool has_operator = false; //(P) is not allowed
        lexeme l = ls.next();
        if (l.type() == lexeme_type::Eof)
        {
            listener->foundMismatchedParanthesis();
            throw unexpected_token_error(l);
        }
        NodePtr root;
        if (l.type() == lexeme_type::LParen)
        {
            has_content = true;
            root = parse_paranthesised_expression(cursor, ls, listener);
        }
        else if (l.type() == lexeme_type::Atom)
        {
            has_content = true;
            root = Node::make_node(l);
        }
        else if (l.type() == lexeme_type::NotOp)
        {
            has_content = true;
            has_operator = true;
            root = Node::make_node(l);
            l = ls.next();

            if (ls.peek().type() == lexeme_type::Atom)
            {
                NodePtr inner = Node::make_node(l);
                lexeme l2 = ls.next();
                if (l2.type() == lexeme_type::RParen)
                {
                    ls.next(); //consume )
                    root->getChildren().emplace_back(inner);
                    return root;
                }
                if (l2.type() == lexeme_type::Eof)
                    listener->foundMismatchedParanthesis();
                else
                    listener->foundUnexpectedToken(l2);
                throw unexpected_token_error(l);
            }
            if (l.type() == lexeme_type::LParen)
            {
                NodePtr inner = parse_paranthesised_expression(cursor, ls, listener);
                root->getChildren().emplace_back(inner);
                return root;
            }
        }
        else if (l.type() == lexeme_type::Atom)
    }

    std::shared_ptr<syntax_tree> strict_parser::parse(const std::vector<lexeme>& lexemes, const std::shared_ptr<parser_listener_t>& listener)
    {
        listener->didStart();
        std::shared_ptr<syntax_tree> result = std::make_shared<syntax_tree>();
        lexeme_stream stream(lexemes);
        lexeme l = stream.peek();
        Cursor c(result,listener);

        switch (l.type())
        {
            case lexeme_type::Eof:
                break; //empty string, nothing to do
            case lexeme_type::Atom:
                {
                    c.new_node(l);
                    listener->didMakeNode(c.get_current_node());
                    if (stream.peek().type() != lexeme_type::Eof)
                    {
                        listener->foundUnexpectedToken(stream.peek());
                        throw unexpected_token_error(stream.peek());
                    }
                    break;
                }
            case lexeme_type::LParen:
                {
                    c.up();
                    NodePtr expr = parse_paranthesised_expression(c, stream, listener);

                    break;
                }
            default:
                throw unexpected_token_error(stream.peek());
        }
        listener->didFinish();

        return result;

    }
}
