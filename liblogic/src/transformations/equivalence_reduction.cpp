//
// Created by xcell on 16.12.2024.
//

#include "equivalence_reduction.h"

#include "node.h"

namespace eloquent::logic {
    bool EquivalenceReduction::match(const NodeObsPtr subtree) {
        return subtree->getType() == NodeType::IffOp;
    }

    /*
     * One should be mindful that equivalence has 2 reduction formulas.
     * The one implemented here is the one which reduces it to its normal negative form.
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

    void EquivalenceReduction::replace(const NodeObsPtr target) {
        NodePtr left_subtree = target->disconnect(0);
        NodePtr right_subtree = target->disconnect(1);

        lexeme l = target->getLexeme();
        target->set_lexeme(lexeme::make(lexeme_type::OrOp, symbols::SYMB_OR, l.start(), l.end()));

        target->spawn_new_child(lexeme::make(lexeme_type::AndOp, symbols::SYMB_AND, 0, 0));
        target->spawn_new_child(lexeme::make(lexeme_type::AndOp, symbols::SYMB_AND, 0, 0));

        target->childAt(1)->spawn_new_child(lexeme::make(lexeme_type::NotOp, symbols::SYMB_NOT, 0, 0));
        target->childAt(1)->spawn_new_child(lexeme::make(lexeme_type::NotOp, symbols::SYMB_NOT, 0, 0));

        NodePtr ls1 = Node::duplicate_node(left_subtree.get());
        NodePtr ls2 = Node::duplicate_node(left_subtree.get());

        NodePtr rs1 = Node::duplicate_node(right_subtree.get());
        NodePtr rs2 = Node::duplicate_node(right_subtree.get());

        target->childAt(0)->adopt(std::move(left_subtree));
        target->childAt(0)->adopt(std::move(right_subtree));

        target->childAt(1)->childAt(0)->adopt(std::move(ls1));
        target->childAt(1)->childAt(0)->adopt(std::move(rs1));

        target->childAt(1)->childAt(1)->childAt(0)->adopt(std::move(ls2));
        target->childAt(1)->childAt(1)->childAt(1)->adopt(std::move(rs2));

    }
}
