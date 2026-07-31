//
// Created by vscode on 7/31/26.
//

#include "strict_parser.hpp"

#include "cursor.h"

namespace eloquent::logic
{
    bool strict_parser::lexeme_stream::can_continue() const noexcept
    {
        return idx < lexemes.size();
    }

    strict_parser::lexeme_stream::lexeme_stream(std::vector<lexeme> lexemes): lexemes(std::move(lexemes))
    {

    }

    lexeme strict_parser::lexeme_stream::peek()
    {
        if (lexemes.empty())
            return lexeme::make(lexeme_type::Eof,"");
        if (idx+1 >= lexemes.size())
            return lexemes[idx];
        return lexemes[idx+1];
    }

    lexeme strict_parser::lexeme_stream::next()
    {
        if (lexemes.empty())
            return lexeme::make(lexeme_type::Eof,"");
        if (idx+1 >= lexemes.size())
            return lexemes[idx];
        return lexemes[++idx];
    }

    syntax_tree strict_parser::parse(const std::vector<lexeme>& lexemes, const std::shared_ptr<parser_listener_t>& listener)
    {
        listener->didStart();
        std::shared_ptr<syntax_tree> result = std::make_shared<syntax_tree>();
        lexeme_stream stream(lexemes);
        lexeme l = stream.peek();
        Cursor c(result);

        switch (l.type())
        {
            case lexeme_type::Eof:
                break; //empty string, nothing to do
            case lexeme_type::Atom: //just one atom
                NodePtr node = Node::make_node(NodeType::Atom, l.token());
                listener->didMakeNode(node);
                c.set_node();
                listener->didPlaceNodeAtCursor(node);
                lis
                if (stream.peek().type() != lexeme_type::Eof)
                    listener->foundUnexpectedToken(stream.peek());
                    throw unexpected_token_error(stream.peek());
                break;
            case lexeme_type::LParen:
                NodePtr expr = parse_paren_expr(stream, listener);
                c.set_node(Node::make_node(NodeType::LParen, expr));
        }
        listener->didFinish();

        return result;

    }
}
