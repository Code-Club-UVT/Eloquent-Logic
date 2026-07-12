//
// Created by xcell on 16.12.2024.
//

#include <liblogic/private/transformations/implication_reduction.h>

namespace eloquent::logic {
    bool ImplicationReduction::match(const NodeObsPtr subtree) {
        const auto node = subtree.lock();
        return node->type == NodeType::ImpliesOp;
    }

    /*
     *        (1)                (1)
     *         |                  |
     *      (\imp)      ->     (\vee)
     *      /   \              /    \
     *    (2)   (3)         (\neg)  (3)
     *                        |
     *                       (2)
     */

    /*
     * This function feels ugly. Maybe it can be rewritten in a nicer way.
     */

    void ImplicationReduction::replace(const NodeObsPtr target) {
        const auto node = target.lock();

        const auto newNode = NodeBuilder::makeNewOrNode();

        // add negation and copy the left branch
        newNode->spawn_new_child();
        newNode->children[0] = NodeBuilder::makeNewNotNode();
        newNode->children[0]->spawn_new_child();
        newNode->children[0]->children[0] = node->children[0];

        // copy right branch as is
        newNode->spawn_new_child();
        newNode->children[1] = node->children[1];

        // give the original node the attributes of the created node
        node->copy_from(newNode);
        node->copy_children(newNode);
    }
}