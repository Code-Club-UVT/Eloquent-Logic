#pragma once
#include <functional>

#include "tree_builder.h"

namespace eloquent::logic {
    struct tree_walker_strategy_base {
        NodeObsPtr root;
        std::function<bool(NodePtr, size_t index)> processNode;
        explicit tree_walker_strategy_base(const NodeObsPtr &root ,std::function<bool(NodePtr, size_t index)> processFunction)
            : root(root), processNode(std::move(processFunction)) {}

        virtual void start() = 0;
        virtual ~tree_walker_strategy_base() = default;
    };
}
