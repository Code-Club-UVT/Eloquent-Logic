//
// Created by xcell on 15.12.2024.
//

#include "double_negation.h"

namespace eloquent::logic {
    bool DoubleNegation::match(const NodeObsPtr subtree, const std::shared_ptr<node_transformation_listener_t>& listener) {
        const auto node = subtree;
        bool result = node->getType() == NodeType::NotOp && node->childAt(0)->getType() == NodeType::NotOp;
        if (result) listener->didMatchDoubleNegation(node);
        return result;
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

    void DoubleNegation::replace(const NodeObsPtr target, const std::shared_ptr<node_transformation_listener_t>& listener) {
        NodePtr new_node = target->childAt(0)->disconnect(0);
        target->set_lexeme(new_node->getLexeme());
        for (size_t i = 0; i < new_node->num_children(); ++i)
        {
            new_node->transfer_child_to(target, i);
        }
        (void)target->disconnect(0);

        listener->didEliminateDoubleNegation(target);
    }

}