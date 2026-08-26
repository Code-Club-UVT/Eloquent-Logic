//
// Created by Bogdan Petru on 09/08/2026.
//

#ifndef ELOQUENTLOGIC_TREE_OPERATION_HPP
#define ELOQUENTLOGIC_TREE_OPERATION_HPP
#include "node_transformation_listener_t.hpp"
#include <node.h>
namespace eloquent {
namespace logic {
class tree_matcher {
  public:
    virtual ~tree_matcher() = default;
    virtual bool
    match(NodeObsPtr subtree,
          const std::shared_ptr<node_transformation_listener_t> &listener) = 0;
};
} // namespace logic
} // namespace eloquent

#endif // ELOQUENTLOGIC_TREE_OPERATION_HPP
