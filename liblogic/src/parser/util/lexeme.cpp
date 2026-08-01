//
// Created by vscode on 7/20/26.
//

#include "lexeme.hpp"

namespace eloquent::logic {
    lexeme lexeme::make(lexeme_type node_type, std::string token, size_t start, size_t end)
    {
        return {node_type, std::move(token), start, end};
    }
    bool is_nary_operator(lexeme_type node_type)
    {
        return node_type == lexeme_type::AndOp
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
