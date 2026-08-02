//
// Created by vscode on 7/20/26.
//

#ifndef ELOQUENTLOGIC_SYNTAX_TREE_HPP
#define ELOQUENTLOGIC_SYNTAX_TREE_HPP

#include <iterator>
#include <memory>
#include <ranges>
#include "node.h"

namespace eloquent::logic {
    class syntax_tree {
        NodePtr m_root = nullptr;
    public:
        NodePtr root();
        void set_root(NodePtr root);
        [[nodiscard]] bool empty() const;
        void extend_upwards(const lexeme& l);
        NodeObsPtr rootRef() const;
        friend bool operator==(const syntax_tree &, const syntax_tree &);
    };
};

#endif //ELOQUENTLOGIC_SYNTAX_TREE_HPP
