//
// Created by vscode on 7/20/26.
//

#include "lexeme.hpp"

namespace eloquent::logic {
    lexeme lexeme::make(lexeme_type node_type, std::string token, size_t start, size_t end)
    {
        return {node_type, std::move(token), start, end};
    }
    bool is_operator(lexeme_type node_type)
    {
        using enum lexeme_type;
        return is_nary_operator(node_type) ||
                node_type == NotOp;
            
    }
    bool is_nary_operator(lexeme_type node_type)
    {
        using enum lexeme_type;
        return node_type == AndOp   ||
            node_type == OrOp       ||
            node_type == IffOp      ||
            node_type == ImpliesOp  ||
            node_type == LEquiOp;
    }
    inline bool is_atom(lexeme_type node_type)
    {
        using enum lexeme_type;
        return node_type == Atom           ||
               node_type == Tautology      ||
               node_type == Contradiction;
    }
    lexeme_type lexeme::type() const {
        return m_node_type;
    }

    std::string lexeme::token() const {
        return m_token;
    }

    size_t lexeme::start() const
    {
        return m_start;
    }

    size_t lexeme::end() const
    {
        return m_end;
    }

}
