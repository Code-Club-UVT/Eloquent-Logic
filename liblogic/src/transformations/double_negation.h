//
// Created by xcell on 15.12.2024.
//

#pragma once
#include <tree_operation_base.h>

namespace eloquent::logic {
    class DoubleNegation : public tree_operation_base {
    public:
        bool match(NodeObsPtr subtree) override;
        void replace(NodeObsPtr target) override;
    };
}
