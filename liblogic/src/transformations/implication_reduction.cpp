//
// Created by xcell on 16.12.2024.
//

#include "implication_reduction.h"

namespace eloquent::logic {
    bool ImplicationReduction::match(const NodeObsPtr subtree) {
        return subtree->getType() == NodeType::ImpliesOp;
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
        const auto node = target;

        NodePtr left_subtree = target->disconnect(0);
        NodePtr right_subtree = target->disconnect(1);
        lexeme l = node->getLexeme();
        node->set_lexeme(lexeme::make(lexeme_type::OrOp, symbols::SYMB_OR, l.start(), l.end()));

        node->spawn_new_child(lexeme::make(lexeme_type::NotOp, symbols::SYMB_NOT, 0,0));
        node->childAt(0)->adopt(std::move(left_subtree));
        node->adopt(std::move(right_subtree));
    }
}