//
// Created by vscode on 7/20/26.
//

#include "syntax_tree.hpp"
namespace eloquent::logic {
NodePtr syntax_tree::root() { return std::move(this->m_root); }

void syntax_tree::set_root(NodePtr root) { this->m_root = std::move(root); }

bool syntax_tree::empty() const { return !this->m_root; }
void syntax_tree::extend_upwards(const lexeme &l) {
    if (empty()) {
        this->m_root = Node::make_node(l);
    } else {
        NodePtr node = this->root();
        this->m_root = Node::make_node(l);
        this->m_root->adopt(std::move(node));
    }
}

NodeObsPtr syntax_tree::rootRef() const { return m_root.get(); }

void syntax_tree::walk(const std::function<void(NodeObsPtr)> &callback) const {
    m_root->walk(callback);
}

bool operator==(const syntax_tree &lhs, const syntax_tree &rhs) {
    return lhs.m_root == rhs.m_root;
}
}; // namespace eloquent::logic