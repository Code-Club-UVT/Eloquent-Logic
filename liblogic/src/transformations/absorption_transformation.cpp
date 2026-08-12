//
// Created by Bogdan Petru on 12/08/2026.
//

#include "absorption_transformation.hpp"

namespace eloquent
{
    namespace logic
    {
        bool absorption_transformation::match(NodeObsPtr subtree, const std::shared_ptr<node_transformation_listener_t>& listener)
        {
            bool result = subtree->getType() == NodeType::AndOp ||
                     subtree->getType() == NodeType::OrOp;
            if (!result) return result;
            NodeObsPtr extra = nullptr;
            if (subtree->getType() == NodeType::AndOp)
            {
                subtree->traverse_children([&](auto node)
                {
                    if (node->getType() == NodeType::OrOp)
                    {
                        extra = node;
                    }
                });
                if (!extra) return false;
                size_t _idx = 0;
                subtree->traverse_children([&](auto node)
                {
                    if (node != extra)
                    {
                        extra->traverse_children([&](auto node2)
                        {
                           if (*node==*node2)
                               result = true;
                        });
                    }
                    if (result)
                        this->idx = _idx;
                    ++_idx;
                });
                return result;
            }
            subtree->traverse_children([&](auto node)
                {
                    if (node->getType() == NodeType::AndOp)
                    {
                        extra = node;
                    }
                });
            if (!extra) return false;
            size_t _idx = 0;
            subtree->traverse_children([&](auto node)
            {
                if (node != extra)
                {
                    extra->traverse_children([&](auto node2)
                    {
                       if (*node==*node2)
                           result = true;
                    });
                }
                if (result)
                    this->idx = _idx;
                ++_idx;
            });
            return result;

        }
        void absorption_transformation::replace(NodeObsPtr target, const std::shared_ptr<node_transformation_listener_t>& listener)
        {
            (void)target->disconnect(idx);
            if (target->num_children() == 1)
            {
                NodePtr node = target->disconnect(0);
                target->set_lexeme(node->getLexeme());
            }
        }
    } // logic
} // eloquent