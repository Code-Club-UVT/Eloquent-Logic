#include <and_op.h>

#include "../parser/util/execution_stack.h"
#include "../operation_private.h"

namespace eloquent::logic {
    bool and_op::execute(std::vector<size_t> &params, const std::weak_ptr<interpretation>& interp, const std::weak_ptr<execution_stack>& estack, const std::string &text) {
        auto stack = estack.lock();
        auto first_node = stack->stack[params[0]];
        bool result = first_node.result.value();
        for (const auto &i : params) {
            auto c_node = stack->stack[i];
            result &= c_node.result.value();
        }
        return result;
    }

}
