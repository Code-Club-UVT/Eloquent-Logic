//
// Created by xcell on 19.12.2024.
//

#pragma once
#include "tree_operation_base.h"
namespace eloquent::logic {
    class Inverter : public tree_operation_base {
    public:
        bool match(NodeObsPtr subtree) override;
        void replace(NodeObsPtr target) override;
    };
}