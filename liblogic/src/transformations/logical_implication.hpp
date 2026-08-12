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
            bool match(NodeObsPtr subtree) override;
            void replace(NodeObsPtr target) override;
        };
    } // logic
} // eloquent

#endif //ELOQUENTLOGIC_LOGICAL_IMPLICATION_HPP
