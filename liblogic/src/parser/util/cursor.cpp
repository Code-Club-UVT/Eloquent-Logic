//
// Created by thonkdifferent on 11.12.24.
//

#include <cursor.h>

namespace eloquent::logic {
    size_t Cursor::find_first_blank_child() const {
        NodePtr c_sharednode = c_node.lock();
        for (size_t i=0; i< c_sharednode->getChildren().size(); i++) {
            if (c_sharednode->getChildren().at(i)->getType() == NodeType::Blank)
                return i;
        }
        return -1;
    }

    void Cursor::up() {
        NodePtr c_sharednode = c_node.lock();
        if (canGoUp()) {
            c_node = c_sharednode->getParent();
        }
    }

    void Cursor::spawn_new_child_node() {
        if (c_node.expired()) {
            tree->set_root(Node::make_node(NodeType::Blank));
            c_node = tree->root();
            return;
        }
        NodePtr c_sharednode = c_node.lock();
        c_sharednode->spawn_new_child();
        c_sharednode->getChildren().back()->getParent() = c_node;
        move_to_first_blank_child();

    }

    void Cursor::move_to_first_blank_child() {
        NodePtr c_sharednode = c_node.lock();
        c_node = c_sharednode->getChildren().at(find_first_blank_child());
    }

    void Cursor::set_node(const NodePtr& n) {
        NodePtr c_sharednode = c_node.lock();
        if (c_node.expired())
        {
            this->tree->set_root(n);
            this->c_node = tree->root();
        }
        if (n->isBlank())
            throw std::invalid_argument("Blank node");
        *c_sharednode=*n;
    }
}