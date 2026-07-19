#pragma once
#include "executor_base.h"

namespace eloquent::logic  {
    struct val_op : executor_base {
        bool execute(std::vector<size_t> &params, const std::weak_ptr<interpretation>& interp, const std::weak_ptr<execution_stack>& estack, const std::string &text = "") override;
    };
}
