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
        void write_to_node(const lexeme& n);

        void up();
        void spawn_new_child_node();
        void grow_up_tree();
        [[nodiscard]] size_t get_arity() const;
        void move_to_child(size_t idx);
        [[nodiscard]] bool has_child(size_t idx) const;
        [[nodiscard]] bool tree_is_empty() const noexcept;
        [[nodiscard]] void replace_child_with_placeholders(lexeme l)
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