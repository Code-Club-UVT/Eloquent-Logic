//
// Created by xcell on 07.02.2025.
//

#include <cassert>
#include "neutral_elements.h"

namespace eloquent::logic {
    NeutralElements::Signal NeutralElements::get_case(const NodeObsPtr& node) {
        
        size_t count_tautology{0};
        size_t count_contradiction{0};

        // something in the back of my mind tells me this is inefficient.
        // good place for future optimizations.

        node->traverse_children([&](auto n){
            if (n->getLexeme().type() == lexeme_type::Tautology) count_tautology++;
            else if (n->getLexeme().type() == lexeme_type::Contradiction) count_contradiction++;
        });

        // case 1 (high priority): a contradiction in a conjunction subtree
        if (node->getType() == NodeType::AndOp && count_contradiction) { return Signal::CONJUNCTION_CONTRADICTION; }

        // case 2 (high priority): a tautology in a disjunction subtree
        if (node->getType() == NodeType::OrOp && count_tautology) { return Signal::DISJUNCTION_TAUTOLOGY; }

        // case 3: a tautology in an n-ary conjunction subtree
        if (node->getType() == NodeType::AndOp && count_tautology ) { return Signal::CONJUNCTION_TAUTOLOGY; }

        // case 4: a contradiction in an n-ary disjunction subtree (dual of
        // case 3). replace()'s drop-loop already handles this signal; it was
        // just never produced here, so e.g. "P or bot" was never simplified.
        if (node->getType() == NodeType::OrOp && count_contradiction) { return Signal::DISJUNCTION_CONTRADICTION; }
        // base case
        return Signal::CLEAR;
    }

    // The following 2 functions could be made more efficient if, instead of the node,
    // we pass them the signal itself, but I feel like this could cause complications in the long run.

    bool NeutralElements::match(const NodeObsPtr subtree, const std::shared_ptr<node_transformation_listener_t>& listener) {
        if (get_case(subtree) != Signal::CLEAR)
        {
            listener->didMatchNeutralElement(subtree);
            return true;
        }
        return false;
    }

    /*
     *          (1)                     (1)                   (1)
     *           |                       |                     |
     *       (\wedge)       ->       (\wedge)       OR        (2)
     *        /    \                     |
     *      (2)   (\top)                (2)                (if binary)
     *
     *          (1)                 (1)
     *           |                   |
     *       (\wedge)       ->    (\bot)
     *       /     \
     *     (2)    (\bot)
     *
     *
     *          (1)                     (1)
     *           |                       |
     *        (\vee)        ->         (\top)
     *        /    \
     *      (2)   (\top)
     *
     *          (1)                 (1)             (1)
     *           |                   |               |
     *        (\vee)        ->    (\vee)    OR      (2)
     *       /     \                 |
     *     (2)    (\bot)            (2)           (if binary)
     */

    void NeutralElements::replace(const NodeObsPtr target, const std::shared_ptr<node_transformation_listener_t>& listener) {

        // It is a good idea to free unused nodes from memory.
        // Although freeing the nodes is optional, since we are using smart pointers,
        // it is a good improvement for larger trees and longer runtimes.
        // The heap will thank the user.

        const auto node = target;
        Signal flag = get_case(node);

        // Neat loop - runs until all unwanted values are cleared.
        // Saves us from running match() and replace() more than once externally.
        // I am unsure whether it may interfere with the wanted order of operations, though.

        while (flag != Signal::CLEAR) {

            if (flag == Signal::CONJUNCTION_CONTRADICTION) {
                // code to free children here
                target->set_lexeme(lexeme::make(lexeme_type::Contradiction, symbols::SYMB_CONTRADICTION, 0,0));
                listener->didTransformNodeWithLexeme(target, target->getLexeme());

                target->clear_children();
                listener->didDiscardAllChildren(target);
                listener->didCollapseToContradiction(target);
            }

            else if (flag == Signal::DISJUNCTION_TAUTOLOGY) {
                target->set_lexeme(lexeme::make(lexeme_type::Tautology, symbols::SYMB_TAUTOLOGY, 0,0));
                listener->didTransformNodeWithLexeme(target, target->getLexeme());
                target->clear_children();
                listener->didDiscardAllChildren(target);
                listener->didCollapseToTautology(target);
            }
            else {
                bool ok = true;
                do
                {
                    size_t delete_idx = 0;
                    ok = true;
                    size_t idx = 0;
                    target->traverse_children([&](auto n)
                    {
                        if ((target->getType() == NodeType::AndOp && n->getLexeme().type() == lexeme_type::Tautology) ||
                           (target->getType() == NodeType::OrOp && n->getLexeme().type() == lexeme_type::Contradiction))
                            delete_idx = idx, ok=false;
                        ++idx;
                    });
                    if (!ok)
                    {
                        (void)target->disconnect(delete_idx);
                        listener->didDisconnect(target, delete_idx);
                        listener->didDropNeutralElement(target);
                    }
                }
                while (!ok);
            }
            flag = get_case(node);
        }
    }

}