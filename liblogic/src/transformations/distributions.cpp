//
// Created by Bogdan Petru on 10/08/2026.
//

#include "distributions.hpp"

namespace eloquent::logic
{
    /*
     * CONJUNCTION distributes over DISJUNCTION (N-ary):
     *
     *            (1)                                (1)
     *             |                                   |
     *          (\wedge)                            (\vee)
     *        /    |    \                        /          \
     *    (o_1) (o_2) (\vee)      ->        (\wedge)        (\wedge)
     *                 /   \               /   |    \      /   |    \
     *              (d_1) (d_2)        (o_1)(o_2)(d_1) (o_1)(o_2)(d_2)
     *
     * Only the first OrOp child found is distributed over - a single call to
     * replace() performs exactly one distribution step. Any remaining OrOp
     * siblings are carried along unchanged inside every new branch, ready for
     * a subsequent match()/replace() pass.
     */

    bool and_distribution::match(NodeObsPtr subtree)
    {
        if (subtree->getType() != NodeType::AndOp) return false;

        bool found = false;
        subtree->traverse_children([&](const NodeObsPtr& child)
        {
            if (child->getType() == NodeType::OrOp) found = true;
        });
        return found;
    }

    void and_distribution::replace(NodeObsPtr target)
    {
        // Detach every child in order. Node::disconnect() is only well-defined
        // at index 0, so we always drain front-to-back rather than indexing in.
        std::vector<NodePtr> others;
        while (target->num_children() > 0)
            others.push_back(target->disconnect(0));

        size_t or_idx = 0;
        while (others[or_idx]->getType() != NodeType::OrOp) ++or_idx;

        NodePtr or_child = std::move(others[or_idx]);
        others.erase(others.begin() + static_cast<std::ptrdiff_t>(or_idx));

        std::vector<NodePtr> disjuncts;
        while (or_child->num_children() > 0)
            disjuncts.push_back(or_child->disconnect(0));

        const lexeme l = target->getLexeme();
        target->set_lexeme(lexeme::make(lexeme_type::OrOp, symbols::SYMB_OR, l.start(), l.end()));

        for (size_t i = 0; i < disjuncts.size(); ++i)
        {
            target->spawn_new_child(lexeme::make(lexeme_type::AndOp, symbols::SYMB_AND, 0, 0));
            const NodeObsPtr conjunction = target->childAt(target->num_children() - 1);

            const bool last = (i + 1 == disjuncts.size());
            for (auto& other : others)
                conjunction->adopt(last ? std::move(other) : Node::duplicate_node(other.get()));

            conjunction->adopt(std::move(disjuncts[i]));
        }
    }

    bool and_distribution::should_apply()
    {
        return false;
    }

    /*
     * DISJUNCTION distributes over CONJUNCTION (N-ary) - mirror image of the above.
     *
     *            (1)                                (1)
     *             |                                   |
     *           (\vee)                             (\wedge)
     *        /    |    \                        /          \
     *    (o_1) (o_2) (\wedge)     ->        (\vee)          (\vee)
     *                 /   \               /   |    \      /   |    \
     *              (c_1) (c_2)        (o_1)(o_2)(c_1) (o_1)(o_2)(c_2)
     */

    bool or_distribution::match(NodeObsPtr subtree)
    {
        if (subtree->getType() != NodeType::OrOp) return false;

        bool found = false;
        subtree->traverse_children([&](const NodeObsPtr& child)
        {
            if (child->getType() == NodeType::AndOp) found = true;
        });
        return found;
    }

    void or_distribution::replace(NodeObsPtr target)
    {
        std::vector<NodePtr> others;
        while (target->num_children() > 0)
            others.push_back(target->disconnect(0));

        size_t and_idx = 0;
        while (others[and_idx]->getType() != NodeType::AndOp) ++and_idx;

        NodePtr and_child = std::move(others[and_idx]);
        others.erase(others.begin() + static_cast<std::ptrdiff_t>(and_idx));

        std::vector<NodePtr> conjuncts;
        while (and_child->num_children() > 0)
            conjuncts.push_back(and_child->disconnect(0));

        const lexeme l = target->getLexeme();
        target->set_lexeme(lexeme::make(lexeme_type::AndOp, symbols::SYMB_AND, l.start(), l.end()));

        for (size_t i = 0; i < conjuncts.size(); ++i)
        {
            target->spawn_new_child(lexeme::make(lexeme_type::OrOp, symbols::SYMB_OR, 0, 0));
            const NodeObsPtr disjunction = target->childAt(target->num_children() - 1);

            const bool last = (i + 1 == conjuncts.size());
            for (auto& other : others)
                disjunction->adopt(last ? std::move(other) : Node::duplicate_node(other.get()));

            disjunction->adopt(std::move(conjuncts[i]));
        }
    }

    bool or_distribution::should_apply()
    {
        return false;
    }
}
