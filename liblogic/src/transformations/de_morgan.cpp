//
// Created by xcell on 16.12.2024.
//

#include <de_morgan.h>

// OPTIMIZATION: due to the high similarity of the classes, they may be fused into one single class.

namespace eloquent::logic {
    bool DeMorganDisjunction::match(const NodeObsPtr subtree) {
        const auto node = subtree.lock();
        return node->type == NodeType::NotOp && node->children[0]->type == NodeType::OrOp;
    }

    /*
     *      (1)              (1)
     *       |                |
     *     (\neg)   ->     (\wedge)
     *       |             /     \
     *     (\vee)       (\neg)  (\neg)
     *     /    \         |       |
     *   (2)    (3)      (2)     (3)
     */

    void DeMorganDisjunction::replace(const NodeObsPtr target) {
        const auto node = target.lock();
        const auto newNode = NodeBuilder::makeNewAndNode();

        for (size_t i = 0; i < node->children[0]->children.size(); i++) {
            newNode->spawn_new_child();
            newNode->children[i] = NodeBuilder::makeNewNotNode();
            newNode->children[i]->spawn_new_child();
            newNode->children[i]->children[0] = node->children[0]->children[i];
        }

        node->copy_from(newNode);
        node->copy_children(newNode);
    }

    bool DeMorganConjunction::match(NodeObsPtr subtree) {
        const auto node = subtree.lock();
        return node->type == NodeType::NotOp && node->children[0]->type == NodeType::AndOp;
    }

    /*
     *      (1)              (1)
     *       |                |
     *     (\neg)   ->      (\vee)
     *       |             /     \
     *    (\wedge)       (\neg)  (\neg)
     *     /    \         |        |
     *   (2)    (3)      (2)      (3)
     */

    void DeMorganConjunction::replace(const NodeObsPtr target) {
        const auto node = target.lock();
        const auto newNode = NodeBuilder::makeNewOrNode();

        for (size_t i = 0; i < node->children[0]->children.size(); i++) {
            newNode->spawn_new_child();
            newNode->children[i] = NodeBuilder::makeNewNotNode();
            newNode->children[i]->spawn_new_child();
            newNode->children[i]->children[0] = node->children[0]->children[i];
        }

        node->copy_from(newNode);
        node->copy_children(newNode);
    }
}