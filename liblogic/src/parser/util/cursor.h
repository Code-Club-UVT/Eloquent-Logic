#pragma once
#include <queue>

#include  "node.h"
#include "syntax_tree.hpp"

namespace eloquent::logic {


    struct Cursor {
        NodeObsPtr c_node;
        std::shared_ptr<syntax_tree> tree;
        void move_to_first_blank_child();
        void set_node(const NodePtr& n);

        void up();
        void spawn_new_child_node();
        [[nodiscard]] bool canGoUp() const noexcept {
            return !c_node.expired();
        }
        [[nodiscard]] size_t find_first_blank_child() const;
        explicit Cursor(std::shared_ptr<syntax_tree> tree): tree(std::move(tree)) {}

    };
};