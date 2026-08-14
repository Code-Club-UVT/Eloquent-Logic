//
// Created by xcell on 16.12.2024.
//

#pragma once
#include "tree_operation_base.h"

namespace eloquent::logic {
    class DeMorganDisjunction : public tree_operation_base {
    public:
        bool match(NodeObsPtr subtree, const std::shared_ptr<node_transformation_listener_t>& listener) override;
        void replace(NodeObsPtr target, const std::shared_ptr<node_transformation_listener_t>& listener) override;
    };

    class DeMorganConjunction : public tree_operation_base {
    public:
        bool match(NodeObsPtr subtree, const std::shared_ptr<node_transformation_listener_t>& listener) override;
        void replace(NodeObsPtr target, const std::shared_ptr<node_transformation_listener_t>& listener) override;
    };
}