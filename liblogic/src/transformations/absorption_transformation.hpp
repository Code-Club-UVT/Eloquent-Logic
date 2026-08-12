//
// Created by Bogdan Petru on 12/08/2026.
//

#ifndef ELOQUENTLOGIC_ABSORPTION_TRANSFORMATION_HPP
#define ELOQUENTLOGIC_ABSORPTION_TRANSFORMATION_HPP

#include "tree_operation_base.h"
namespace eloquent
{
    namespace logic
    {
        class absorption_transformation : public tree_operation_base
        {
            size_t idx = 0;
        public:
            bool match(NodeObsPtr subtree) override;
            void replace(NodeObsPtr target) override;
        };


    } // logic
} // eloquent

#endif //ELOQUENTLOGIC_ABSORPTION_TRANSFORMATION_HPP
