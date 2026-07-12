#include <liblogic/private/operations/or_op.h>

#include "liblogic/private/execution_stack.h"
#include "liblogic/private/operation_private.h"

namespace eloquent::logic {
    bool or_op::execute(std::vector<size_t> &params, const std::weak_ptr<interpretation>& interp, const std::weak_ptr<execution_stack>& estack, const std::string &text) {
        auto stack = estack.lock();
        auto first_node = stack->stack[params[0]];
        bool result = first_node.result.value();
        for (const auto &i : params) {
            result |= stack->stack[i].result.value();
        }
        return result;
    }

}
