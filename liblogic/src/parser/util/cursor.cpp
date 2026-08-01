//
// Created by thonkdifferent on 11.12.24.
//

#include <cursor.h>

namespace eloquent::logic {
    void Cursor::new_node(const lexeme& l)
    {
        NodeObsPtr new_node = nullptr;
        if (this->tree->empty()) //root
        {
            this->tree->extend_upwards(l);

        }
        else
        {
            switch (this->direction)
            {
                using enum Direction;
                case Up:
                    {
                        this->tree->extend_upwards(l);
                    }
                case Down:
                    {
                        this->c_node->spawn_new_child(l);

                    }
                case Current:
                    {
                        listener->didTrySpawiningOverExistingNode(this->cursor_position);
                        throw std::logic_error("Cursor::move_to_first_child");
                    }

            }
        }
        this->direction = Direction::Current;
        listener->didPlaceNode();
    }

    size_t Cursor::find_first_blank_child() const {
        NodeObsPtr c_sharednode = c_node ;
        for (size_t i=0; i< c_sharednode->getChildren().size(); i++) {
            if (c_sharednode->getChildren().at(i)->getType() == NodeType::Blank)
                return i;
        }
        return -1;
    }

    void Cursor::up() {
        if (cursor_position.direction == Position::Up)
        {
            listener->didTryInvalidPosition();
            throw std::logic_error("Cursor::up");
        };
        NodeObsPtr c_sharednode = c_node ;
        c_node = c_sharednode->getParent();

    }

    void Cursor::spawn_new_child_node() {
        if (c_node != nullptr) return;
        NodeObsPtr c_sharednode = c_node ;
        c_sharednode->spawn_new_child(lexeme::make(lexeme_type::Unknown,"", 0,0));
        move_to_first_blank_child();

    }

    void Cursor::move_to_first_blank_child() {
        NodeObsPtr c_sharednode = c_node ;
        c_node = c_sharednode->getChildren().at(find_first_blank_child());
    }

    void Cursor::write_to_node(const lexeme& n) {
        if (c_node != nullptr)
        {
            throw std::logic_error("Cursor::write_to_node");
        }
        NodeObsPtr c_sharednode = c_node ;

        if (!c_sharednode->isBlank())
            throw std::invalid_argument("Can't write to not blank node");
        c_sharednode->set_lexeme(n);
    }
}