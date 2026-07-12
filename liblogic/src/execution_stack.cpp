#include <liblogic/private/execution_stack.h>



namespace eloquent::logic
{

    std::weak_ptr<std::set<std::string>> execution_stack::get_atoms() const
    {
        return atoms;
    }
    void execution_stack::add_atom(const std::string& atom)
    {
        atoms->insert(atom);
    }
    void execution_stack::append_operation(const operation_private& op)
    {
        stack.push_back(op);
    }

    void execution_stack::build(const NodeObsPtr& root, std::weak_ptr<execution_stack> estack)
    {
        if (!stack.empty())
            return;
        auto root_ptr = root.lock();
        // ReSharper disable once CppDFAConstantFunctionResult
        auto callback = [this, &estack](const NodePtr& node, const size_t index)
        {
            if (node->type == NodeType::Atom)
            {
                auto& node_text = node->text;
                if (node_text.size() == 3 && std::isdigit(node_text.back()))
                {
                    node_text = std::string({node_text[0],node_text[1],'{',node_text[2],'}'});
                }
                this->add_atom(node_text);
            }
            //if this is the first node, add it
            if (this->stack.empty())
            {
                this->append_operation(operation_private(node,estack,node->text));
            }

            this->stack[this->stack.size()-1].arguments.resize(node->children.size());
            for (size_t i = 0; i<node->children.size(); i++)
            {
                this->append_operation(operation_private(node->children[i],estack, node->children[i]->text,
                    result_position_t(index, i)));
            }
            return true;
        };

        const luke_tree_walker walker(make_breath_first_traversal(root,callback));
        walker.walk();
    }

    bool execution_stack::execute(const std::weak_ptr<interpretation>& interpretation)
    {
        size_t index = stack.size()-1;
        for (auto it = stack.rbegin(); it!= stack.rend(); ++it, index--)
        {

            if (it->evaluated)
               continue;
            auto new_index = index;
            do
            {
                stack[new_index].evaluate(interpretation);
                if (stack[new_index].result_position != std::nullopt)
                {
                    auto _pos = stack[new_index].result_position;
                    stack[_pos->node_index].arguments[_pos->operand_index] = new_index;
                    new_index = _pos->node_index;
                }
                else break;
            }
            while (stack[new_index].can_execute());
        }
        return stack.begin()->result.value();
    }

    void execution_stack::reset()
    {
        for (auto& item: stack)
        {
            item.result = std::nullopt;
            item.evaluated = false;
        }
    }
}
