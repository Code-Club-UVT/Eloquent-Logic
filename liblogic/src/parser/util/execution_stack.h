#pragma once
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "../../operation_private.h"
#include <tree_walker.h>
#include <breath_first_traversal.h>

namespace eloquent::logic {

    class execution_stack {
    public:
        std::vector<operation_private> stack;
        std::shared_ptr<std::set<std::string>> atoms = std::make_shared<std::set<std::string>>();

        [[nodiscard]] std::weak_ptr<std::set<std::string>> get_atoms() const;
        void add_atom(const std::string& atom);
        void append_operation(const operation_private& op);
        void build(const NodeObsPtr& root, std::weak_ptr<execution_stack> estack);
        bool execute(const std::weak_ptr<interpretation>& interpretation);
        void reset();
        explicit execution_stack() = default;
    };
}
