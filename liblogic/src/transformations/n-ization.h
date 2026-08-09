//
// Created by xcell on 17.12.2024.
//

#pragma once
#include "tree_operation_base.h"
#include <vector>
#include <node.h>

namespace eloquent::logic {
    class N_izer : public tree_operation_base {
        std::vector<NodePtr> get_descendants(const NodeObsPtr& subtree);
    public:
        bool match(NodeObsPtr subtree) override;
        void replace(NodeObsPtr target) override;
    };
}