//
// Created by xcell on 07.02.2025.
//

#include <cassert>
#include <liblogic/private/transformations/neutral_elements.h>

namespace eloquent::logic {
    NeutralElements::Signal NeutralElements::get_case(const NodeObsPtr& node) {

        const auto locked = node.lock();

        size_t count_tautology{0};
        size_t count_contradiction{0};

        // something in the back of my mind tells me this is inefficient.
        // good place for future optimizations.

        for (const auto& n : locked->children) {
            if (n->text == ELOQUENT_LIBLOGIC_SYMB_TAUTOLOGY) count_tautology++;
            else if (n->text == ELOQUENT_LIBLOGIC_SYMB_CONTRADICTION) count_contradiction++;
        }

        // case 1 (high priority): a contradiction in a conjunction subtree
        if (locked->type == NodeType::AndOp && count_contradiction) { return Signal::SIG_CONJUNCTION_CONTRADICTION; }

        // case 2 (high priority): a tautology in a disjunction subtree
        if (locked->type == NodeType::OrOp && count_tautology) { return Signal::SIG_DISJUNCTION_TAUTOLOGY; }

        // case 3: a tautology in a binary conjunction subtree
        if (locked->type == NodeType::AndOp && count_tautology && locked->children.size() == 2) { return Signal::SIG_CONJUNCTION_TAUTOLOGY_BINARY; }

        // case 4: a tautology in an n-ary conjunction subtree
        if (locked->type == NodeType::AndOp && count_tautology && locked->children.size() > 2) { return Signal::SIG_CONJUNCTION_TAUTOLOGY_MULTIPLE; }

        // case 5: a contradiction in a binary disjunction subtree
        if (locked->type == NodeType::OrOp && count_contradiction && locked->children.size() == 2) { return Signal::SIG_DISJUNCTION_CONTRADICTION_BINARY; }

        // case 6: a contradiction in an n-ary disjunction subtree
        if (locked->type == NodeType::OrOp && count_contradiction && locked->children.size() > 2) { return Signal::SIG_DISJUNCTION_CONTRADICTION_MULTIPLE; }

        // base case
        return Signal::SIG_CLEAR;
    }

    // The following 2 functions could be made more efficient if, instead of the node,
    // we pass them the signal itself, but I feel like this could cause complications in the long run.

    bool NeutralElements::match(const NodeObsPtr subtree) {
        if (get_case(subtree) != Signal::SIG_CLEAR) return true;
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

    void NeutralElements::replace(const NodeObsPtr target) {

        // It is a good idea to free unused nodes from memory.
        // Although freeing the nodes is optional, since we are using smart pointers,
        // it is a good improvement for larger trees and longer runtimes.
        // The heap will thank the user.

        const auto node = target.lock();
        Signal flag = get_case(node);

        // Neat loop - runs until all unwanted values are cleared.
        // Saves us from running match() and replace() more than once externally.
        // I am unsure whether it may interfere with the wanted order of operations, though.

        while (flag != Signal::SIG_CLEAR) {

            if (flag == Signal::SIG_CONJUNCTION_CONTRADICTION) {
                // code to free children here

                const auto newnode = NodeBuilder::makeNewContradictionNode();
                node->copy_children(newnode);
                node->copy_from(newnode);
            }

            if (flag == Signal::SIG_DISJUNCTION_TAUTOLOGY) {
                // code to free children here

                const auto newnode = NodeBuilder::makeNewTautologyNode();
                node->copy_children(newnode);
                node->copy_from(newnode);
            }

            if (flag == Signal::SIG_CONJUNCTION_TAUTOLOGY_BINARY ||
                flag == Signal::SIG_DISJUNCTION_CONTRADICTION_BINARY) {

                auto newnode = NodeBuilder::makeNewBlankNode();
                size_t node_index{0};

                if (node->children[0]->text == ELOQUENT_LIBLOGIC_SYMB_TAUTOLOGY ||
                    node->children[0]->text == ELOQUENT_LIBLOGIC_SYMB_CONTRADICTION) {
                    node_index = 1;
                }

                newnode->copy_children(node->children[node_index]);
                newnode->copy_from(node->children[node_index]);

                // code to free children here

                // I'm using an intermediary node here because I want to make
                // sure that nothing breaks after copy_children() is called.

                node->copy_children(newnode);
                node->copy_from(newnode);
            }

            if (flag == Signal::SIG_CONJUNCTION_TAUTOLOGY_MULTIPLE) {
                // We want to get rid of all tautologies. If, somehow, all but one node were tautologies,
                // we copy that into the parent node. If no nodes remain, we know that the parent must be a tautology.
                std::erase_if(node->children, [](const NodeObsPtr& n) {
                    const auto l = n.lock();
                    return l->text == ELOQUENT_LIBLOGIC_SYMB_TAUTOLOGY;
                });

                if (node->children.empty()) {
                    const auto newnode = NodeBuilder::makeNewTautologyNode();
                    node->copy_children(newnode);
                    node->copy_from(newnode);
                }

                if (node->children.size() == 1) {
                    const auto newnode = node->children[0];
                    node->copy_children(newnode);
                    node->copy_from(newnode);
                }
            }

            if (flag == Signal::SIG_DISJUNCTION_CONTRADICTION_MULTIPLE) {
                // We want to get rid of all contradictions. If, somehow, all but one node were contradictions,
                // we copy that into the parent node. If no nodes remain, we know that the parent must be a contradiction.
                std::erase_if(node->children, [](const NodeObsPtr& n) {
                    const auto l = n.lock();
                    return l->text == ELOQUENT_LIBLOGIC_SYMB_CONTRADICTION;
                });

                if (node->children.empty()) {
                    const auto newnode = NodeBuilder::makeNewContradictionNode();
                    node->copy_children(newnode);
                    node->copy_from(newnode);
                }

                if (node->children.size() == 1) {
                    const auto newnode = node->children[0];
                    node->copy_children(newnode);
                    node->copy_from(newnode);
                }
            }

            flag = get_case(node);
        }

        // If the node is a binary operation node, and it has less than 2 children, it's broken.
        assert( !( (node->type == NodeType::AndOp || node->type == NodeType::OrOp) && node->children.size() < 2 ) );

    }

}