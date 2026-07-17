#include <iff_op.h>

#include "../parser/util/execution_stack.h"
#include "../operation_private.h"

namespace eloquent::logic {
    bool iff_op::execute(std::vector<size_t> &params, const std::weak_ptr<interpretation>& interp, const std::weak_ptr<execution_stack>& estack, const std::string &text) {
        auto stack = estack.lock();
        auto n1 = stack->stack[params[0]];
        auto n2 = stack->stack[params[1]];
        return n1.result.value() == n2.result.value();
    }

}
