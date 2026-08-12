#include "../operation_private.h"

#include <utility>

#include <execution_stack.h>

namespace eloquent::logic {
    operation_private::operation_private(NodeObsPtr _node, std::weak_ptr<execution_stack> _estack,std::string  node_text, const std::optional<result_position_t> &rpos) : result_position(rpos), node_text(std::move(node_text)),estack(_estack){
        auto node_ref = _node.lock();
        switch (node_ref->type) {
            using enum NodeType;
            case Atom:
                executor = std::make_shared<val_op>();
                this->representation = node_text;
            break;
            case NotOp:
                executor = std::make_shared<not_op>();
            break;
            case AndOp:
                executor = std::make_shared<and_op>();
            break;
            case OrOp:
                executor = std::make_shared<or_op>();
            break;
            case ImpliesOp:
                executor = std::make_shared<implies_op>();
            break;
            case IffOp:
                executor = std::make_shared<iff_op>();
            break;
            default:
                throw std::runtime_error("Invalid node type");
        }
        prepare_arguments(node_ref->children.size());
    }
    void operation_private::prepare_arguments(size_t argc) {
        this->arguments.reserve(argc);
    }
    void operation_private::reset() {
        for (size_t i = 0; i < this->arguments.size(); i++) {
            this->evaluated = false;
        }
    }


    void operation_private::evaluate(const std::weak_ptr<interpretation>& interp) {
        auto stack = estack.lock();
        if (this->representation.empty())
        {
            if (this->arguments.empty())
                this->representation = this->node_text;
            else if (this->arguments.size() == 1)
                this->representation = std::string(R"((\\neg)")+stack->stack[this->arguments[0]].representation + ")";
            else
            {
                this->representation = "(";
                for (size_t i = 0; i< arguments.size()-1; i++)
                {
                    this->representation += (stack->stack[this->arguments[i]].representation + this->node_text);
                }
                this->representation += stack->stack[this->arguments.back()].representation + ')';
            }
        }
        evaluated = true;
        this->result= this->executor->execute(this->arguments,interp,estack, node_text);
    }
    bool operation_private::can_execute() const
    {
        for (const auto& arg: arguments)
        {
            auto stack = estack.lock();
            if (stack->stack[arg].result== std::nullopt)
                return false;
        }
        return true;
    }


}
