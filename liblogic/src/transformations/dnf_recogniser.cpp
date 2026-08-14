//
// Created by Bogdan Petru on 11/08/2026.
//

#include "dnf_recogniser.hpp"

namespace eloquent
{
    namespace logic
    {
        namespace
        {
            // A literal is a bare atom or its negation (NotOp wrapping
            // exactly one Atom child) — NNF guarantees negation never
            // appears any deeper than this.
            bool is_literal(NodeObsPtr node)
            {
                if (node->getType() == NodeType::Atom)
                {
                    return true;
                }
                return node->getType() == NodeType::NotOp
                    && node->num_children() == 1
                    && node->childAt(0)->getType() == NodeType::Atom;
            }

            // A DNF term is a conjunction of literals, or — since a term of
            // exactly one literal has no AndOp wrapper — a bare literal.
            bool is_term(NodeObsPtr node)
            {
                if (is_literal(node))
                {
                    return true;
                }
                if (node->getType() != NodeType::AndOp)
                {
                    return false;
                }
                bool result = true;
                node->traverse_children([&](auto child)
                {
                    result &= is_literal(child);
                });
                return result;
            }
        }

        bool dnf_recogniser::match(NodeObsPtr subtree, const std::shared_ptr<node_transformation_listener_t>& listener)
        {
            // DNF is a disjunction of terms, or — since a formula of
            // exactly one term has no OrOp wrapper — a bare term.
            bool result;
            if (subtree->getType() == NodeType::OrOp)
            {
                result = true;
                subtree->traverse_children([&](auto node1)
                {
                    result &= is_term(node1);
                });
            }
            else
            {
                result = is_term(subtree);
            }
            listener->didCheckDNF(subtree, result);
            return result;
        }
    } // logic
} // eloquent
