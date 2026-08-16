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
            const NodeType extra_type = subtree->getType() == NodeType::AndOp ? NodeType::OrOp : NodeType::AndOp;

            NodeObsPtr extra = nullptr;
            size_t extra_idx = 0;
            {
                size_t scan_idx = 0;
                subtree->traverse_children([&](auto node)
                {
                    if (node->getType() == extra_type)
                    {
                        extra = node;
                        extra_idx = scan_idx;
                    }
                    ++scan_idx;
                });
            }
            if (!extra) return false;

            // `result` was only used above to confirm subtree is an AndOp/OrOp;
            // it must be reset before reusing it to record whether a redundant
            // (absorbable) element was actually found among the siblings.
            result = false;
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
            });
            if (result)
            {
                this->idx = extra_idx;
                listener->didMatchAbsorption(subtree);
            }
            return result;
        }
        void absorption_transformation::replace(NodeObsPtr target, const std::shared_ptr<node_transformation_listener_t>& listener)
        {
            (void)target->disconnect(idx);
            listener->didDisconnect(target, idx);
            if (target->num_children() == 1)
            {
                NodePtr node = target->disconnect(0);
                listener->didDisconnect(target, 0);
                target->set_lexeme(node->getLexeme());
                listener->didTransformNodeWithLexeme(target, target->getLexeme());
            }

            listener->didApplyAbsorption(target);
        }
    } // logic
} // eloquent