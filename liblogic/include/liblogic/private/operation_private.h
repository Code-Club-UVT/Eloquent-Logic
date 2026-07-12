#pragma once
#include <memory>
#include <optional>
#include <utility>
#include <vector>
#include "interpretation.h"
#include "tree_builder.h"
#include "operations/operations.h"

namespace eloquent::logic {
    class execution_stack;
}

namespace eloquent::logic {

    struct result_position_t {
        size_t node_index;
        size_t operand_index;

        friend bool operator==(const result_position_t &lhs, const result_position_t &rhs) {
            return lhs.node_index == rhs.node_index
                   && lhs.operand_index == rhs.operand_index;
        }

        friend bool operator!=(const result_position_t &lhs, const result_position_t &rhs) {
            return !(lhs == rhs);
        }

        explicit result_position_t(size_t node_index, size_t operand_index) : node_index(node_index), operand_index(operand_index) {}
    };
    struct operation_private {
        std::optional<result_position_t>  result_position;
        std::shared_ptr<executor_base> executor;
        std::vector<size_t> arguments;
        std::string representation;
        std::string node_text;
        std::weak_ptr<execution_stack> estack;
        bool evaluated = false;
        std::optional<bool> result = std::nullopt;

        friend bool operator==(const operation_private &lhs, const operation_private &rhs) {
            return lhs.result_position == rhs.result_position
                   && lhs.arguments == rhs.arguments
                   && lhs.representation == rhs.representation
                   && lhs.node_text == rhs.node_text;
        }

        friend bool operator!=(const operation_private &lhs, const operation_private &rhs) {
            return !(lhs == rhs);
        }

        explicit operation_private(NodeObsPtr _node, std::weak_ptr<execution_stack> _estack,std::string  node_text, const std::optional<result_position_t>& rpos= std::nullopt);
        void prepare_arguments(size_t argc);
        void reset();
        void evaluate(const std::weak_ptr<interpretation>& interp);
        [[nodiscard]] bool can_execute() const;
    };

    class operation {
        operation_private op;

    public:
        explicit operation(NodeObsPtr _node, std::weak_ptr<execution_stack> _estack, const std::string& text, const std::optional<result_position_t>& rpos = std::nullopt) : op(std::move(_node), std::move(_estack),text, rpos) {};
        void evaluate(const std::weak_ptr<interpretation>& interp, const std::string& text){
            return op.evaluate(interp);
        }
        void reset() {
            op.reset();
        }
    };


}
