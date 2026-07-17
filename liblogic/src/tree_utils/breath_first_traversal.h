#pragma once
#include <functional>

#include "tree_walker_strategy_base.h"
#include <queue>
namespace eloquent::logic {
    struct breath_first_traversal : tree_walker_strategy_base {
        void start() override;
        std::queue<std::tuple<size_t, NodeObsPtr>> queue;

        explicit breath_first_traversal(const NodeObsPtr &root,
            const std::function<bool(NodePtr, size_t index)> &processFunction)
            : tree_walker_strategy_base(root, processFunction) {
            queue.emplace(0, root);
        }
    };
    std::unique_ptr<breath_first_traversal> make_breath_first_traversal(const NodeObsPtr &root, const std::function<bool(NodePtr, size_t index)> &processFunction);

}
