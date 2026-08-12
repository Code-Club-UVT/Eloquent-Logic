//
// Created by Bogdan Petru on 11/08/2026.
//

#ifndef ELOQUENTLOGIC_CNF_RECOGNISER_HPP
#define ELOQUENTLOGIC_CNF_RECOGNISER_HPP
#include "tree_matcher.hpp"
namespace eloquent
{
    namespace logic
    {
        class cnf_recogniser : public tree_matcher
        {
        public:
            bool match(NodeObsPtr subtree, const std::shared_ptr<node_transformation_listener_t>& listener) final;
        };
    } // logic
} // eloquent

#endif //ELOQUENTLOGIC_CNF_RECOGNISER_HPP
