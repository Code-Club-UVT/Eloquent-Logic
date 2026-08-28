//
// Created by xcell on 16.12.2024.
//

#include "equivalence_reduction.h"

#include "node.h"

namespace eloquent::logic {
bool EquivalenceReduction::match(
    const NodeObsPtr subtree,
    const std::shared_ptr<node_transformation_listener_t> &listener) {
    bool result = subtree->getType() == NodeType::IffOp;
    if (result)
        listener->didMatchEquivalence(subtree);
    return result;
}

/*
 * One should be mindful that equivalence has 2 reduction formulas.
 * The one implemented here is the one which reduces it to its normal negative
 * form.
 *
 *       (1)                (1)
 *        |                  |
 *      (\iff)      ->     (\vee)
 *      /   \              /    \
 *     (2)  (3)           /      \
 *                   (\wedge)   (\wedge)
 *                    /    \     /      \
 *                  (2)    (3)  (\neg)  (\neg)
 *                                |       |
 *                               (2)     (3)
 */

/*
 * Another yucky implementation. Please look into beautifying this.
 *
 * WARNING: if programs break during or after simplifications of equivalences,
 *          it may mean the pointers don't play nice with each other. Check
 *          how the spawned children nodes inherit and instead of assigning,
 *          have them copy the children of the input node.
 */

void EquivalenceReduction::replace(
    const NodeObsPtr target,
    const std::shared_ptr<node_transformation_listener_t> &listener) {
    NodePtr left_subtree = target->disconnect(0);
    listener->didDisconnect(target, 0);
    // The right child shifts down to index 0 once the left child is
    // removed, so this must disconnect(0), not disconnect(1).
    NodePtr right_subtree = target->disconnect(0);
    listener->didDisconnect(target, 0);

    lexeme l = target->getLexeme();
    target->set_lexeme(
        lexeme::make(lexeme_type::OrOp, symbols::SYMB_OR, l.start(), l.end()));
    listener->didTransformNodeWithLexeme(target, target->getLexeme());

    target->spawn_new_child(
        lexeme::make(lexeme_type::AndOp, symbols::SYMB_AND, 0, 0));
    listener->didSpawnNewSubtree(target->lastChild());
    listener->didAdoptNode(target, target->lastChild());

    target->spawn_new_child(
        lexeme::make(lexeme_type::AndOp, symbols::SYMB_AND, 0, 0));
    listener->didSpawnNewSubtree(target->lastChild());
    listener->didAdoptNode(target, target->lastChild());

    target->childAt(1)->spawn_new_child(
        lexeme::make(lexeme_type::NotOp, symbols::SYMB_NOT, 0, 0));
    listener->didSpawnNewSubtree(target->childAt(1)->lastChild());
    listener->didAdoptNode(target->childAt(1), target->childAt(1)->lastChild());

    target->childAt(1)->spawn_new_child(
        lexeme::make(lexeme_type::NotOp, symbols::SYMB_NOT, 0, 0));
    listener->didSpawnNewSubtree(target->childAt(1)->lastChild());
    listener->didAdoptNode(target->childAt(1), target->childAt(1)->lastChild());

    // The AndOp(l,r) branch consumes the originals; the NotOp(l), NotOp(r)
    // branch needs its own copies of the operands.
    NodePtr ls = Node::duplicate_node(left_subtree.get());
    listener->didSpawnNewSubtree(ls.get());

    NodePtr rs = Node::duplicate_node(right_subtree.get());
    listener->didSpawnNewSubtree(rs.get());

    target->childAt(0)->adopt(std::move(left_subtree));
    listener->didAdoptNode(target->childAt(0), target->childAt(0)->lastChild());
    target->childAt(0)->adopt(std::move(right_subtree));
    listener->didAdoptNode(target->childAt(0), target->childAt(0)->lastChild());

    target->childAt(1)->childAt(0)->adopt(std::move(ls));
    listener->didAdoptNode(target->childAt(1)->childAt(0),
                           target->childAt(1)->childAt(0)->lastChild());
    target->childAt(1)->childAt(1)->adopt(std::move(rs));
    listener->didAdoptNode(target->childAt(1)->childAt(1),
                           target->childAt(1)->childAt(1)->lastChild());

    listener->didReduceEquivalence(target);
}
} // namespace eloquent::logic
