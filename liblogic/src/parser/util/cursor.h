#pragma once
#include <queue>

#include  "node.h"
#include "syntax_tree.hpp"

namespace eloquent::logic {


    struct CursorPrivate {
        NodeObsPtr c_node;
        std::shared_ptr<syntax_tree> tree;
        void move_to_first_blank_child();
        void set_node(NodePtr n);

        void up();
        void spawn_new_child_node();
        [[nodiscard]] bool canGoUp() const noexcept {
            return !c_node.expired();
        }
        [[nodiscard]] size_t find_first_blank_child() const;
        explicit CursorPrivate(std::shared_ptr<syntax_tree> tree): tree(std::move(tree)) {}

    };


    struct Cursor {
        void set_node(NodePtr n) {
            _cursor.set_node(std::move(n));
        }
        void up() {
            _cursor.up();
        }
        void down() {
            _cursor.move_to_first_blank_child();
        }
        void spawn_node() {
            _cursor.spawn_new_child_node();
        }
        CursorPrivate _cursor;
    };
};