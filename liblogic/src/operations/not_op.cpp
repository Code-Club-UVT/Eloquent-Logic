#include <liblogic/private/operations/not_op.h>

#include "liblogic/private/execution_stack.h"
#include "liblogic/private/operation_private.h"

namespace eloquent::logic {
    bool not_op::execute(std::vector<size_t> &params, const std::weak_ptr<interpretation>& interp, const std::weak_ptr<execution_stack>& estack, const std::string &text)
    {
        auto stack = estack.lock();
        auto node = stack->stack[params[0]];
        return !node.result.value();
    }
}
