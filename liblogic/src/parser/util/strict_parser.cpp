//
// Created by vscode on 7/31/26.
//

#include "strict_parser.hpp"
#include "unexpected_token_error.hpp"
#include "cursor.h"

namespace eloquent::logic
{


    std::shared_ptr<syntax_tree> strict_parser::parse(const std::vector<lexeme>& lexemes, const std::shared_ptr<parser_listener_t>& listener)
    {
        listener->didStart();
        std::shared_ptr<syntax_tree> result = std::make_shared<syntax_tree>();
        lexeme_stream stream(lexemes);
        Cursor cursor(result,listener);
        lexeme l = stream.current();
        while (stream.can_continue())
        {
            switch (l.type())
            {
                using enum lexeme_type;
            case  LParen: // 2 posibilitati
                {
                    listener->startedProcessingParanthesis();
                    bool unary  = stream.peek().type() == NotOp;
                    bool binary = stream.peek().type() == LParen || stream.peek().type() == Atom;
                    if (!unary && !binary)
                        throw unexpected_token_error(l);

                    if (cursor.tree_is_empty())
                    {
                        unary ? listener->acceptUnaryOpVariant() : listener->acceptBinaryOpVariant();
                        cursor.grow_up_tree();
                        cursor.spawn_new_child_node();
                        if (binary) { cursor.spawn_new_child_node(); cursor.move_to_child(0); }
                    }
                    else
                    {
                        unary ? listener->acceptUnaryOpVariant() : listener->acceptBinaryOpVariant();
                        lexeme arity_hint = lexeme::make(unary ? NotOp : AndOp, "", 0, 0);
                        cursor.replace_child_with_placeholders(arity_hint);
                        if (binary) cursor.move_to_child(0);
                    }
                    break;
                }
            case RParen:
                {
                    NodeObsPtr ptr = cursor.get_current_node();
                    if (ptr->isRoot() && ptr->isBlank())
                    {
                        listener->foundAstError(cursor);
                        throw unexpected_token_error(l);
                    }
                    if (!ptr->allChildrenAreWritten())
                    {
                        listener->foundAstError(cursor);
                        throw unexpected_token_error(l);
                    }
                    cursor.up();
                    break;
                }
            case Atom:
                {
                    if (cursor.get_tree()->empty())
                        cursor.grow_up_tree();
                    if (cursor.get_arity() != 0)
                    {
                        listener->wrongArityForNode(cursor.get_current_node());
                        throw unexpected_token_error(l);
                    }
                    cursor.write_to_node(l);
                    cursor.up();
                    break;
                }
            case AndOp:
            case OrOp:
            case ImpliesOp:
            case IffOp:
            case LEquiOp:
                {
                    if (cursor.get_current_node() == nullptr)
                    {
                        listener->didTryInvalidPosition(nullptr, 0);
                        throw unexpected_token_error(l);
                    }
                    if (!cursor.get_current_node()->isBlank())
                    {
                        listener->foundAstError(cursor);
                        throw unexpected_token_error(l);
                    }
                    cursor.write_to_node(l);
                    if (!cursor.has_child(1))
                    {
                        listener->wrongArityForNode(cursor.get_current_node());
                        throw unexpected_token_error(l);
                    }
                    cursor.move_to_child(1); //right child
                    break;
                }
            case NotOp:
                {
                    if (cursor.get_current_node() == nullptr)
                    {
                        listener->didTryInvalidPosition(nullptr, 0);
                        throw unexpected_token_error(l);
                    }
                    if (!cursor.get_current_node()->isBlank())
                    {
                        listener->foundAstError(cursor);
                        throw unexpected_token_error(l);
                    }
                    if (cursor.get_arity() != 1)
                    {
                        listener->wrongArityForNode(cursor.get_current_node());
                        throw unexpected_token_error(l);
                    }
                    cursor.write_to_node(l);
                    cursor.move_to_child(0);
                    break;
                }
            case Eof:
                {
                    if (cursor.get_current_node() != nullptr)
                    {
                        listener->foundAstError(cursor);
                        throw unexpected_token_error(l);
                    }
                    break;
                }
            default:
                {
                    listener->foundUnexpectedToken(l);
                    throw unexpected_token_error(l);
                    break;
                }
            }
            l = stream.next();
        }
        listener->didFinish();

        return result;

    }
}
