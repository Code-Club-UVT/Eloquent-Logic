#include <val_op.h>
namespace eloquent::logic {
    bool val_op::execute(std::vector<size_t> &params, const std::weak_ptr<interpretation>& interp, const std::weak_ptr<execution_stack>& estack, const std::string &text)
    {
        auto interp_ref = interp.lock();

        if (text == R"(\\top)") //tautology
            return true;
        if (text == R"(\\bot)") //contradiction
            return false;
        return interp_ref->at(text);
    }
}
