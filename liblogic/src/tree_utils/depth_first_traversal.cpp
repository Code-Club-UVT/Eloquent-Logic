#include <depth_first_traversal.h>

namespace eloquent::logic {
    void depth_first_traversal::start() {
        while (!stack.empty()) {
            auto [index,node] = this->stack.top();
            this->stack.pop();

            if (node.expired())
                return;

            const auto c_node = node.lock();
            if (!processNode(c_node,index))
                return;

            size_t j = c_node->children.size()-1 ;
            for (auto i = c_node->children.rbegin(); i != c_node->children.rend(); ++i, --j) {
                this->stack.emplace(j, *i);
            }
        }

    }
    std::unique_ptr<depth_first_traversal> make_depth_first_traversal(const NodeObsPtr& root, const std::function<bool(NodePtr,size_t)>& processFunction) {
        return std::make_unique<depth_first_traversal>(root, processFunction);
    }

};