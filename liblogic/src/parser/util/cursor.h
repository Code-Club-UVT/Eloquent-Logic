#pragma once
#include <queue>
#include <utility>

#include "cursor_listener_t.hpp"
#include  "node.h"
#include "syntax_tree.hpp"

namespace eloquent::logic {


    class Cursor {
        NodeObsPtr c_node;
        std::shared_ptr<syntax_tree> tree;
        std::shared_ptr<cursor_listener_t> listener;

        enum class Direction
        {
            Up,
            Current,
            Down
        } direction;
    public:
        void move_to_first_blank_child();
        void write_to_node(const lexeme& n);

        void up();
        void spawn_new_child_node();
        void new_node(const lexeme& l );
        [[nodiscard]] size_t find_first_blank_child() const;
        explicit Cursor(std::shared_ptr<syntax_tree> tree, std::shared_ptr<cursor_listener_t> listener): tree(std::move(tree)), listener(std::move(listener)) {}

        [[nodiscard]] NodeObsPtr get_current_node() const
        {
            return c_node;
        }

        [[nodiscard]] std::shared_ptr<syntax_tree> get_tree() const
        {
            return tree;
        }
    };
};