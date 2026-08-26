//
// Created by xcell on 16.12.2024.
//

#include "implication_reduction.h"

namespace eloquent::logic {
bool ImplicationReduction::match(
    const NodeObsPtr subtree,
    const std::shared_ptr<node_transformation_listener_t> &listener) {
    bool result = subtree->getType() == NodeType::ImpliesOp;
    if (result)
        listener->didMatchImplication(subtree);
    return result;
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

void ImplicationReduction::replace(
    const NodeObsPtr target,
    const std::shared_ptr<node_transformation_listener_t> &listener) {
    const auto node = target;

    NodePtr left_subtree = target->disconnect(0);
    listener->didDisconnect(target, 0);
    // The right child shifts down to index 0 once the left child is
    // removed, so this must disconnect(0), not disconnect(1).
    NodePtr right_subtree = target->disconnect(0);
    listener->didDisconnect(target, 0);

    lexeme l = node->getLexeme();
    node->set_lexeme(
        lexeme::make(lexeme_type::OrOp, symbols::SYMB_OR, l.start(), l.end()));
    listener->didTransformNodeWithLexeme(node, node->getLexeme());

    node->spawn_new_child(
        lexeme::make(lexeme_type::NotOp, symbols::SYMB_NOT, 0, 0));
    listener->didSpawnNewSubtree(node->lastChild());
    listener->didAdoptNode(node, node->lastChild());

    node->childAt(0)->adopt(std::move(left_subtree));
    listener->didSpawnNewSubtree(node->childAt(0)->lastChild());
    listener->didAdoptNode(node->childAt(0), node->childAt(0)->lastChild());
    node->adopt(std::move(right_subtree));
    listener->didAdoptNode(node, node->lastChild());

    listener->didReduceImplication(node);
}
} // namespace eloquent::logic