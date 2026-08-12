//
// Created by Bogdan Petru on 11/08/2026.
//

#include "cnf_recogniser.hpp"

namespace eloquent
{
    namespace logic
    {
        bool cnf_recogniser::match(NodeObsPtr subtree, const std::shared_ptr<node_transformation_listener_t>& listener)
        {
            bool result = subtree->getType() == NodeType::AndOp;
            subtree->traverse_children([&](auto node1)
            {
                result&=(node1->getType() == NodeType::OrOp || node1->getType() == NodeType::Atom);
                node1->traverse_children([&](auto node2)
                {
                    result&=(node2->getType() == NodeType::Atom);
                });
            });
            return result;
        }
    } // logic
} // eloquent