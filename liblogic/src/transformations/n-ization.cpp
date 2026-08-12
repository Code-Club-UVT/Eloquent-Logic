//
// Created by xcell on 17.12.2024.
//

#include <n-ization.h>

namespace eloquent::logic {

    // I wish Clang-Tidy wouldn't yell at me for having a recursive call chain.
    // I know the function call stack isn't infinite, but still...
    std::vector<NodePtr> N_izer::get_descendants(const NodeObsPtr& subtree) {
        const auto node = subtree.lock();
        std::vector<NodePtr> descendants;

        // Classical recursive approach:
        // if we meet a child of the same operation as the parent, we get its children until there are no more.
        for (const auto& child : node->children) {
            if (child->type == node->type) {
                std::vector<NodePtr> local_children = get_descendants(child);
                descendants.insert(descendants.end(), local_children.begin(), local_children.end());
            } else { descendants.push_back(child); }
        }

        return descendants;
    }

    /*
     *  The reason I'm iterating through all children, is because I'm considering the possibility
     *  of more than 2 children, which, in all honesty, is unlikely, but I'm paranoid like that.
     */

    bool N_izer::match(const NodeObsPtr subtree) {
        const auto node = subtree.lock();

        if (const auto type = node->type; type == NodeType::AndOp || type == NodeType::OrOp) {
            for (size_t i = 0; i < node->children.size(); i++) {
                if (node->children[i]->type == type) {
                    return true;
                }
            }
        }

        return false;
    }

    /*
     *  The way ANTLR builds the tree is done in such a way that there is only one conjunction/disjunction
     *  per subtree level. Consider simplifying this function later or modifying the tree building mechanism
     *  altogether.
     *
     *             (1)                            (1)
     *              |                              |
     *          (AND/OR)                        (AND/OR)
     *          /      \            ->        /  |   |  \
     *     (AND/OR)   (AND/OR)              (2) (3) (4) (5)
     *      /   \      /    \
     *    (2)   (3)  (4)    (5)
     */

    void N_izer::replace(const NodeObsPtr target) {
        const auto node = target.lock();

        // The classic American strategy - outsourcing the labor to someone that does it better.
        // I needed a function which specifically returned a vector of children.
        const std::vector<NodePtr> accumulated_children = get_descendants(target);

        node->children = accumulated_children;
    }

}