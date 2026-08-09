//
// Created by Bogdan Petru on 09/08/2026.
//

#ifndef ELOQUENTLOGIC_TREE_OPERATION_HPP
#define ELOQUENTLOGIC_TREE_OPERATION_HPP
#include <node.h>
namespace eloquent
{
    namespace logic
    {
        class tree_matcher
        {
        public:
            virtual ~tree_matcher() = default;

        private:
            virtual bool match(NodeObsPtr subtree) = 0;

        };
    } // logic
} // eloquent

#endif //ELOQUENTLOGIC_TREE_OPERATION_HPP
