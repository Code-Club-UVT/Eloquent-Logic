//
// Created by vscode on 7/20/26.
//

#ifndef ELOQUENTLOGIC_SYNTAX_TREE_HPP
#define ELOQUENTLOGIC_SYNTAX_TREE_HPP

#include <iterator>
#include <memory>
#include <generator>
#include <ranges>
#include "tree_builder.h"

namespace eloquent::logic {
    class syntax_tree {
        NodePtr m_root = nullptr;
        std::generator<NodePtr> post_order_impl(NodePtr node) const;
    public:
        NodePtr root();
        void set_root(NodePtr root);
        std::generator<NodePtr> post_order() const;
        friend bool operator==(const syntax_tree &, const syntax_tree &);
    };
};

#endif //ELOQUENTLOGIC_SYNTAX_TREE_HPP
