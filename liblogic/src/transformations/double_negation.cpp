//
// Created by xcell on 15.12.2024.
//

#include <liblogic/private/transformations/double_negation.h>

namespace eloquent::logic {
    bool DoubleNegation::match(const NodeObsPtr subtree) {
        const auto node = subtree.lock();
        return node->type == NodeType::NotOp && node->children[0]->type == NodeType::NotOp;
    }

    /*
     *   (1)
     *    |
     *  (\neg)              (1)
     *    |         ->       |
     *  (\neg)              (2)
     *    |
     *   (2)
     */

    void DoubleNegation::replace(const NodeObsPtr target) {
        const auto node = target.lock();
        const auto newNode = node->children[0]->children[0];
        node->copy_children(newNode);
        node->copy_from(newNode);
    }

}