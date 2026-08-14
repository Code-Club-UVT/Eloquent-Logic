#include "convert.hpp"

#include <syntax_tree.hpp>

namespace logic_agent::types
{
    namespace
    {
        lexeme_type_t to_dto(eloquent::logic::lexeme_type t)
        {
            using eloquent::logic::lexeme_type;
            switch (t)
            {
                case lexeme_type::Atom: return lexeme_type_t::Atom;
                case lexeme_type::Tautology: return lexeme_type_t::Tautology;
                case lexeme_type::Contradiction: return lexeme_type_t::Contradiction;
                case lexeme_type::LParen: return lexeme_type_t::LParen;
                case lexeme_type::RParen: return lexeme_type_t::RParen;
                case lexeme_type::NotOp: return lexeme_type_t::NotOp;
                case lexeme_type::AndOp: return lexeme_type_t::AndOp;
                case lexeme_type::OrOp: return lexeme_type_t::OrOp;
                case lexeme_type::ImpliesOp: return lexeme_type_t::ImpliesOp;
                case lexeme_type::IffOp: return lexeme_type_t::IffOp;
                case lexeme_type::LEquiOp: return lexeme_type_t::LEquiOp;
                case lexeme_type::Eof: return lexeme_type_t::Eof;
                case lexeme_type::Unknown: default: return lexeme_type_t::Unknown;
            }
        }

        node_type_t to_dto(eloquent::logic::NodeType t)
        {
            using eloquent::logic::NodeType;
            switch (t)
            {
                case NodeType::Atom: return node_type_t::Atom;
                case NodeType::NotOp: return node_type_t::NotOp;
                case NodeType::AndOp: return node_type_t::AndOp;
                case NodeType::OrOp: return node_type_t::OrOp;
                case NodeType::ImpliesOp: return node_type_t::ImpliesOp;
                case NodeType::IffOp: return node_type_t::IffOp;
                case NodeType::LEquiOp: return node_type_t::LEquiOp;
                case NodeType::Blank: default: return node_type_t::Blank;
            }
        }
    }

    uuid_t to_dto(const CppCommon::UUID& id)
    {
        return uuid_t{id.string()};
    }

    lexeme_t to_dto(const eloquent::logic::lexeme& l)
    {
        return lexeme_t{
            .type = to_dto(l.type()),
            .token = l.token(),
            .start = l.start(),
            .end = l.end(),
        };
    }

    node_t to_dto(const eloquent::logic::Node& node)
    {
        node_t dto{
            .id = to_dto(node.getUUID()),
            .type = to_dto(node.getType()),
            .lexeme = to_dto(node.getLexeme()),
            .children = {},
        };
        dto.children.reserve(node.num_children());
        for (size_t i = 0; i < node.num_children(); ++i)
        {
            dto.children.push_back(to_dto(node.childAt(i)->getUUID()));
        }
        return dto;
    }

    std::optional<node_t> to_dto(eloquent::logic::NodeObsPtr node)
    {
        if (node == nullptr)
        {
            return std::nullopt;
        }
        return to_dto(*node);
    }

    parser::cursor_t to_dto(const eloquent::logic::Cursor& cursor)
    {
        parser::cursor_t dto{
            .current_node = to_dto(cursor.get_current_node()),
            .tree_root = std::nullopt,
        };
        if (const auto tree = cursor.get_tree())
        {
            dto.tree_root = to_dto(tree->rootRef());
        }
        return dto;
    }
}
