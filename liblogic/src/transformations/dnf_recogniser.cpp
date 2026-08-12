//
// Created by Bogdan Petru on 11/08/2026.
//

#include "dnf_recogniser.hpp"

namespace eloquent
{
    namespace logic
    {
        bool dnf_recogniser::match(NodeObsPtr subtree)
        {
            bool result = subtree->getType() == NodeType::OrOp;
            subtree->traverse_children([&](auto node1)
            {
                result&=(node1->getType() == NodeType::AndOp || node1->getType() == NodeType::Atom);
                node1->traverse_children([&](auto node2)
                {
                    result&=(node2->getType() == NodeType::Atom);
                });
            });
            return result;
        }
    } // logic
} // eloquent