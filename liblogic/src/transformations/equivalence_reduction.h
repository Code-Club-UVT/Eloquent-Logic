//
// Created by xcell on 16.12.2024.
//

#pragma once
#include <node.h>
#include "tree_operation_base.h"

namespace eloquent::logic {
    class EquivalenceReduction : public tree_operation_base {
    public:
        bool match(NodeObsPtr subtree) override;
        void replace(NodeObsPtr target) override;
    };
}