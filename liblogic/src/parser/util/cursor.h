#pragma once
#include <queue>

#include "execution_stack.h"

namespace eloquent::logic {


    struct CursorPrivate {
        NodeObsPtr c_node;
        NodePtr root = nullptr;
        std::shared_ptr<execution_stack> estack = std::make_shared<execution_stack>();
        void down();
        void set_node(NodePtr n);

        void make_root();
        void up();

        void spawn_new_child_node();

        [[nodiscard]] bool are_trees_equal(const CursorPrivate  &other) const {
            return *root == *other.root;
        }
        [[nodiscard]] bool canGoUp() const noexcept {
            return !c_node.expired();
        }
        [[nodiscard]] bool isEmpty() const noexcept {
            return root == nullptr;
        }
        size_t find_first_blank_child() const;
        [[nodiscard]] std::optional<bool> evaluate(const std::weak_ptr<interpretation>& interpretation) {
            if (root == nullptr) {return std::nullopt;}
            estack->build(root,estack);
            estack->execute(interpretation);
            return estack->stack[0].result;
        }
    };


    struct Cursor {
        void set_node(NodePtr n) {
            _cursor.set_node(std::move(n));
        }
        void up() {
            _cursor.up();
        }
        void down() {
            _cursor.down();
        }
        void spawn_node() {
            _cursor.spawn_new_child_node();
        }
        std::optional<bool> evaluate(const std::weak_ptr<interpretation>& interpretation) {
            return _cursor.evaluate(interpretation);
        }
        CursorPrivate _cursor;
    };
};