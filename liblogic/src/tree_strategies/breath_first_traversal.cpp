#include <liblogic/private/tree_strategies/breath_first_traversal.h>

namespace eloquent::logic {
    void breath_first_traversal::start() {
        while (!queue.empty()) {
            auto [index,node] = queue.front();
            queue.pop();
            if (node.expired())
                return;
            const auto c_node = node.lock();
            if (!processNode(c_node,index))
                break;

            for (size_t i = 0; i < c_node->children.size(); i++) {
                queue.emplace(i, c_node->children.at(i));
            }
        }
    }
    std::unique_ptr<breath_first_traversal> make_breath_first_traversal(const NodeObsPtr& root, const std::function<bool(NodePtr,size_t)>& processFunction) {
        return std::make_unique<breath_first_traversal>(root, processFunction);
    }
}