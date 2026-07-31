//
// Created by vscode on 7/20/26.
//

#include "lexeme.hpp"

namespace eloquent::logic {
    lexeme_type make_from_node(NodeType nt)
    {
        switch (nt)
        {
            case NodeType::Atom:
                return lexeme_type::Atom;
            case NodeType::NotOp:
            return lexeme_type::NotOp;
                break;
            case NodeType::AndOp:
            return lexeme_type::AndOp;
                break;
            case NodeType::OrOp:
            return lexeme_type::OrOp;
                break;
            case NodeType::ImpliesOp:
            return lexeme_type::ImpliesOp;
                break;
            case NodeType::IffOp:
            return lexeme_type::IffOp;
                break;
            case NodeType::LEquiOp:
            return lexeme_type::LEquiOp;
                break;
            case NodeType::Blank:
                return lexeme_type::Unknown;
                break;
        }
        return lexeme_type::Unknown;
    }

    lexeme lexeme::make(lexeme_type node_type, std::string token, size_t start, size_t end)
    {
        return {node_type, std::move(token), start, end};
    }

    lexeme_type lexeme::type() const {
        return m_node_type;
    }

    std::string lexeme::token() const {
        return m_token;
    }

    size_t lexeme::position() const
    {
        return m_position;
    }
}
