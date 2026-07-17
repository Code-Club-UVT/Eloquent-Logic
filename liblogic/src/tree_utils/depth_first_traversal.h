#pragma once
#include <functional>

#include <tree_walker_strategy_base.h>
#include <stack>
namespace eloquent::logic {


    struct depth_first_traversal : tree_walker_strategy_base {

        std::stack<std::tuple<size_t,NodeObsPtr>> stack;
        explicit depth_first_traversal(const NodeObsPtr& root, const std::function<bool(NodePtr,size_t)>& processFunction) : tree_walker_strategy_base(root,processFunction) {
            stack.emplace(0, root);
        };
        void start() override;
    };
    std::unique_ptr<depth_first_traversal> make_depth_first_traversal(const NodeObsPtr& root, const std::function<bool(NodePtr,size_t)>& processFunction);
}