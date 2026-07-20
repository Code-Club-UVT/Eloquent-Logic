//
// Created by vscode on 7/20/26.
//

#include "lexeme.hpp"

namespace eloquent::logic {
    lexeme lexeme::build_from_node(NodeObsPtr ptr) {
        if (ptr.expired())
            throw std::invalid_argument("null node");
        NodePtr node_ptr = ptr.lock();
        return lexeme(node_ptr->type, node_ptr->text);
    }

    NodeType lexeme::node_type() const {
        return m_node_type;
    }

    std::string lexeme::token() const {
        return m_token;
    }
}