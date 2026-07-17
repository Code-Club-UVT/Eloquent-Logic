//
// Created by thonkdifferent on 11.12.24.
//

#include <cursor.h>

namespace eloquent::logic {
    size_t CursorPrivate::find_first_blank_child() const {
        NodePtr c_sharednode = c_node.lock();
        for (size_t i=0; i< c_sharednode->children.size(); i++) {
            if (c_sharednode->children[i]->type == NodeType::Blank)
                return i;
        }
        return -1;
    }

    void CursorPrivate::up() {
        NodePtr c_sharednode = c_node.lock();
        if (canGoUp()) {
            c_node = c_sharednode->parent;
        }
    }

    void CursorPrivate::spawn_new_child_node() {
        if (root == nullptr)
            make_root();
        else {
            NodePtr c_sharednode = c_node.lock();
            c_sharednode->spawn_new_child();
            c_sharednode->children.back()->parent = c_node;
            down();
        }
    }

    void CursorPrivate::down() {
        NodePtr c_sharednode = c_node.lock();
        c_node = c_sharednode->children.at(find_first_blank_child());
    }
    void CursorPrivate::make_root() {
        root = NodeBuilder::makeNewBlankNode();
        c_node = root;
    }

    void CursorPrivate::set_node(const NodePtr n) {
        NodePtr c_sharednode = c_node.lock();
        if (n->isBlank())
            throw std::invalid_argument("Blank node");
        c_sharednode->copy_from(n);
    }
}