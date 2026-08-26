//
// Created by xcell on 16.12.2024.
//

#include "de_morgan.h"

// OPTIMIZATION: due to the high similarity of the classes, they may be fused
// into one single class.

namespace eloquent::logic {

bool DeMorganDisjunction::match(
    const NodeObsPtr subtree,
    const std::shared_ptr<node_transformation_listener_t> &listener) {
    const auto node = subtree;
    bool result = node->getType() == NodeType::NotOp &&
                  node->childAt(0)->getType() == NodeType::OrOp;
    if (result)
        listener->didMatchDeMorganDisjunction(node);
    return result;
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

void DeMorganDisjunction::replace(
    const NodeObsPtr target,
    const std::shared_ptr<node_transformation_listener_t> &listener) {
    const auto node = target;

    node->set_lexeme(lexeme::make(lexeme_type::AndOp, symbols::SYMB_AND,
                                  node->getLexeme().start(),
                                  node->getLexeme().end()));

    NodePtr cChild = node->disconnect(0);
    for (size_t i = 0; i < cChild->num_children(); ++i) {
        node->spawn_new_child(
            lexeme::make(lexeme_type::NotOp, symbols::SYMB_NOT, 0, 0));
        cChild->transfer_child_to(node->childAt(i), i);
    }

    listener->didApplyDeMorganDisjunction(node);
}

bool DeMorganConjunction::match(
    NodeObsPtr subtree,
    const std::shared_ptr<node_transformation_listener_t> &listener) {
    const auto node = subtree;
    bool result = node->getType() == NodeType::NotOp &&
                  node->childAt(0)->getType() == NodeType::AndOp;
    if (result)
        listener->didMatchDeMorganConjunction(node);
    return result;
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

void DeMorganConjunction::replace(
    const NodeObsPtr target,
    const std::shared_ptr<node_transformation_listener_t> &listener) {
    const auto node = target;

    node->set_lexeme(lexeme::make(lexeme_type::OrOp, symbols::SYMB_OR,
                                  node->getLexeme().start(),
                                  node->getLexeme().end()));

    NodePtr cChild = node->disconnect(0);
    for (size_t i = 0; i < cChild->num_children(); ++i) {
        node->spawn_new_child(
            lexeme::make(lexeme_type::NotOp, symbols::SYMB_NOT, 0, 0));
        cChild->transfer_child_to(node->childAt(i), i);
    }

    listener->didApplyDeMorganConjunction(node);
}
} // namespace eloquent::logic