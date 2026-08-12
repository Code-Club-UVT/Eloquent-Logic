//
// Created by Bogdan Petru on 10/08/2026.
//

#ifndef ELOQUENTLOGIC_AND_DISTRIBUTION_HPP
#define ELOQUENTLOGIC_AND_DISTRIBUTION_HPP
#include "tree_operation_base.h"
namespace eloquent::logic
{
    class and_distribution : public tree_operation_base
    {
    public:
        bool match(NodeObsPtr subtree) final;
        void replace(NodeObsPtr target) final;
        bool should_apply() final;

    };

    class or_distribution : public tree_operation_base
    {
    public:
        bool match(NodeObsPtr subtree) final;
        void replace(NodeObsPtr target) final;
        bool should_apply() final;
    };
}


#endif //ELOQUENTLOGIC_AND_DISTRIBUTION_HPP
