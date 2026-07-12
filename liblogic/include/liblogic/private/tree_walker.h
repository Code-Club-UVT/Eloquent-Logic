#pragma once
#include <memory>

#include "tree_builder.h"
#include "tree_strategies/tree_walker_strategy_base.h"

namespace eloquent::logic {
    struct luke_tree_walker {
        std::unique_ptr<tree_walker_strategy_base> strategy;
        void walk() const;
        explicit luke_tree_walker(std::unique_ptr<tree_walker_strategy_base> strategy): strategy(std::move(strategy)) {};
    };
}
