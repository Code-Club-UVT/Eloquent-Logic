//
// Created by Bogdan Petru on 27/08/2026.
//

#ifndef ELOQUENTLOGIC_LOGICAL_CONSEQUENCE_HPP
#define ELOQUENTLOGIC_LOGICAL_CONSEQUENCE_HPP
#include <memory>

#include "logical_consequence_listener_t.hpp"
#include "mapper_listener.hpp"
#include "node_transformation_listener_t.hpp"
#include "sat_listener.hpp"
#include "syntax_tree.hpp"

namespace eloquent::logic {
class node_transformation_listener_t;

class logical_consequence {
public:
    static bool solve_logical_consequence(
        std::shared_ptr<syntax_tree> &tree,
        const std::shared_ptr<logical_consequence_listener_t> &listener,
        const std::shared_ptr<node_transformation_listener_t> &node_listener,
        const std::shared_ptr<sat_listener> &sat_listener,
        const std::shared_ptr<mapper_listener> &mapper_listener);
};
} // namespace eloquent::logic

#endif // ELOQUENTLOGIC_LOGICAL_CONSEQUENCE_HPP
