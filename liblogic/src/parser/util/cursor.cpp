//
// Created by thonkdifferent on 11.12.24.
//

#include <cursor.h>

namespace eloquent::logic {
    void Cursor::grow_up_tree()
    {
        lexeme l = lexeme::make(lexeme_type::Unknown,"", 0,0);
        this->tree->extend_upwards(l);
        this->c_node = tree->rootRef();
        listener->didPlaceNode();
    }

    size_t Cursor::get_arity() const
    {
        return c_node->arity();
    }

    bool Cursor::has_child(size_t idx) const
    {
        return idx < c_node->num_children();
    }

    bool Cursor::tree_is_empty() const noexcept
    {
        return this->tree->empty();
    }

    void Cursor::replace_child_with_placeholders(lexeme l)
    {
        NodeObsPtr parent = c_node->getParent();
        if (parent == nullptr)
        {
            grow_up_tree();
            if (is_nary_operator(l.type()))
            {
                spawn_new_child_node();
                up();
            }
            return;
        }
        NodePtr root = Node::make_node(lexeme::make(lexeme_type::Unknown,"", 0,0));
        root->spawn_new_child(lexeme::make(lexeme_type::Unknown,"", 0,0));
        if (is_nary_operator(l.type()))
        {
            root->spawn_new_child(lexeme::make(lexeme_type::Unknown,"", 0,0));
        }
        size_t node_idx = parent->find_by_uuid(c_node->getUUID());
        parent->replace_child(node_idx, std::move(root));
        c_node = parent->childAt(node_idx);
    }

    void Cursor::move_to_child(size_t idx)
    {
        if (!has_child(idx))
        {
            listener->didTryInvalidPosition(c_node, idx);
            return;
        }
        this->c_node = this->c_node->childAt(idx);
    }


    void Cursor::up() {
        if (direction == Direction::Up)
        {
            listener->alreadyUp();
            throw std::logic_error("Cursor::up");
        };
        if (c_node->getParent() == nullptr)
            this->direction = Direction::Up;
        else
            this->c_node = c_node->getParent();

        this->listener->didGoUp();

    }

    void Cursor::spawn_new_child_node() {
        if (c_node != nullptr) return;
        c_node->spawn_new_child(lexeme::make(lexeme_type::Unknown,"", 0,0));
        listener->spawnedNewChildNode(c_node, c_node->childAt(c_node->num_children()-1));
    }


    void Cursor::write_to_node(const lexeme& n) {
        if (c_node == nullptr)
        {
            throw std::logic_error("Cursor::write_to_node");
        }
        NodeObsPtr c_sharednode = c_node ;

        if (!c_sharednode->isBlank())
            throw std::invalid_argument("Can't write to not blank node");
        c_sharednode->set_lexeme(n);
    }
}