//
// Created by vscode on 7/31/26.
//

#include "relaxed_parser.hpp"
#include "unexpected_token_error.hpp"
#include "lexeme_stream.hpp"
#include <map>
namespace eloquent::logic
{
    struct binding_power
    {
        size_t left, right;
        binding_power(size_t power): left(power), right(power+1) {}
    };
    constexpr size_t default_power = 0;
    static std::map<lexeme_type, binding_power> binding_map = {
        {lexeme_type::AndOp, binding_power{10}},
        {lexeme_type::OrOp, binding_power{9}},
        {lexeme_type::ImpliesOp, binding_power{8}},
        {lexeme_type::IffOp, binding_power{7}},
        {lexeme_type::LEquiOp, binding_power{6}},
    };

    NodePtr expr(size_t min_binding, lexeme_stream& stream, const std::shared_ptr<relaxed_parser_listener_t>& listener);

    void disallow_pure_atoms_in_parens(lexeme_stream& stream, const std::shared_ptr<relaxed_parser_listener_t>& listener)
    {
        if (stream.peek(1).type() == lexeme_type::Atom &&
            stream.peek(2).type() == lexeme_type::RParen)
        {
            listener->didFindPureAtomInParens(stream.current(),stream.peek(1),stream.peek(2));
            throw unexpected_token_error(stream.peek(2));
        }
    }

    NodePtr handle_parens(lexeme_stream& stream, const std::shared_ptr<relaxed_parser_listener_t>& listener)
    {
        listener->startParsingParens(stream.current());
        disallow_pure_atoms_in_parens(stream, listener);
        NodePtr root = expr(default_power, stream, listener);
        auto l = stream.peek();
        if ( l.type() != lexeme_type::RParen)
        {
            listener->mismatchedParens(l);
            throw unexpected_token_error(l);
        }
        return root;
    }

    NodePtr parse_nud(lexeme_stream& stream, const std::shared_ptr<relaxed_parser_listener_t>& listener, lexeme& l)
    {
        NodePtr lhs;
        switch (l.type()) //lhs poate fi: P, ~P, ~(, (
        {
            using enum lexeme_type;
        case LParen:
            {
                lhs = handle_parens(stream, listener);
                break;
            }
        case Atom:
            {
                lhs = Node::make_node(l); // atomic node
                listener->didMakeNewSubtree(lhs.get());
                break;
            }
        case NotOp:
            {
                lhs = Node::make_node(l);
                listener->didMakeNewSubtree(lhs.get());
                l = stream.next();
                NodePtr subtree = parse_nud(stream, listener, l);
                lhs->adopt(std::move(subtree));
                listener->didJoin(lhs.get(), subtree.get());
                break;
            }
        default:
            listener->foundUnexpectedToken(l);
            throw unexpected_token_error(l);
        }
        return lhs;
    }

    NodePtr expr(size_t min_binding, lexeme_stream& stream, const std::shared_ptr<relaxed_parser_listener_t>& listener)
    {
        lexeme l = stream.next();
        listener->didReadLexeme(l);
        NodePtr lhs = parse_nud(stream, listener, l);

        while (true)
        {
            l = stream.peek();
            listener->didReadLexeme(l);
            if (l.type() == lexeme_type::Eof || l.type() == lexeme_type::RParen)
                return lhs;
            if (is_nary_operator(l.type()))
            {
                lexeme op = lexeme::make(l.type(), l.token(), l.start(), l.end());
                l = stream.next();
                listener->didReadLexeme(l);
                binding_power bp = binding_map.at(l.type());
                if (bp.left < min_binding)
                {
                    listener->didFindLowerPrecendenceOperator(l);
                    break;
                }
                NodePtr root = Node::make_node(l);
                listener->didMakeNewSubtree(root.get());
                root->adopt(std::move(lhs));
                listener->didJoin(root.get(), lhs.get());
                root->adopt(expr(bp.right, stream, listener));
                listener->didJoin(root.get(), lhs.get());
                lhs.swap(root);
            }
            else
            {
                listener->foundUnexpectedToken(l);
                throw unexpected_token_error(l);
            }
        }
        return lhs;
    }
    std::shared_ptr<syntax_tree> relaxed_parser::parse(const std::vector<lexeme>& lexemes, const std::shared_ptr<relaxed_parser_listener_t>& listener)
    {
        listener->didStart();
        std::shared_ptr<syntax_tree> result = std::make_shared<syntax_tree>();
        lexeme_stream stream(lexemes);
        lexeme l = stream.current();
        if (l.type() != lexeme_type::Eof)
        {
            NodePtr root = expr(default_power, stream, listener);
            result->set_root(std::move(root));
        }
        listener->didFinish();

        return result;

    }
}