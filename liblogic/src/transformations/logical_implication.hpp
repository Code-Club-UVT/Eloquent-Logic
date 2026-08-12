//
// Created by Bogdan Petru on 10/08/2026.
//

#ifndef ELOQUENTLOGIC_LOGICAL_IMPLICATION_HPP
#define ELOQUENTLOGIC_LOGICAL_IMPLICATION_HPP
#include "tree_operation_base.h"
namespace eloquent
{
    namespace logic
    {
        class logical_implication : public tree_operation_base
        {
        public:
            bool match(NodeObsPtr subtree, const std::shared_ptr<node_transformation_listener_t>& listener) override;
            void replace(NodeObsPtr target, const std::shared_ptr<node_transformation_listener_t>& listener) override;
        };
    } // logic
} // eloquent

#endif //ELOQUENTLOGIC_LOGICAL_IMPLICATION_HPP
