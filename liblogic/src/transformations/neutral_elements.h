//
// Created by xcell on 07.02.2025.
//

#pragma once
#include "tree_operation_base.h"

namespace eloquent::logic {
    class NeutralElements : public tree_operation_base {
        enum class Signal {
            CLEAR = 0,
            CONJUNCTION_CONTRADICTION,          // high priority
            DISJUNCTION_TAUTOLOGY,              // high priority
            CONJUNCTION_TAUTOLOGY,
            DISJUNCTION_CONTRADICTION,
        };
    public:
        static Signal get_case(const NodeObsPtr& node);
        bool match(NodeObsPtr subtree, const std::shared_ptr<node_transformation_listener_t>& listener) override;
        void replace(NodeObsPtr target, const std::shared_ptr<node_transformation_listener_t>& listener) override;
    };
}
