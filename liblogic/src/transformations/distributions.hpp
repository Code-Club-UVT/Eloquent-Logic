//
// Created by Bogdan Petru on 10/08/2026.
//

#ifndef ELOQUENTLOGIC_AND_DISTRIBUTION_HPP
#define ELOQUENTLOGIC_AND_DISTRIBUTION_HPP
#include "tree_operation_base.h"
namespace eloquent::logic {
class and_distribution : public tree_operation_base {
  public:
    bool match(
        NodeObsPtr subtree,
        const std::shared_ptr<node_transformation_listener_t> &listener) final;
    void replace(
        NodeObsPtr target,
        const std::shared_ptr<node_transformation_listener_t> &listener) final;
    bool should_apply() final;
};

class or_distribution : public tree_operation_base {
  public:
    bool match(
        NodeObsPtr subtree,
        const std::shared_ptr<node_transformation_listener_t> &listener) final;
    void replace(
        NodeObsPtr target,
        const std::shared_ptr<node_transformation_listener_t> &listener) final;
    bool should_apply() final;
};
} // namespace eloquent::logic

#endif // ELOQUENTLOGIC_AND_DISTRIBUTION_HPP
