#pragma once
#include <vector>
#include <stdexcept>
#include <memory>
#include <../parser/util/interpretation.h>

//#include "liblogic/private/execution_stack.h"

namespace eloquent::logic {
    class execution_stack;
    struct operation_private;

    struct executor_base {
        virtual bool execute(std::vector<size_t> &params, const std::weak_ptr<interpretation>& interp, const std::weak_ptr<execution_stack>& estack, const std::string &text = "") = 0;
        virtual ~executor_base() = default;
    };
}
