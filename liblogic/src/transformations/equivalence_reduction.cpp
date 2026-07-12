//
// Created by xcell on 16.12.2024.
//

#include <liblogic/private/transformations/equivalence_reduction.h>

namespace eloquent::logic {
    bool EquivalenceReduction::match(const NodeObsPtr subtree) {
        const auto node = subtree.lock();
        return node->type == NodeType::IffOp;
    }

    /*
     * One should be mindful that equivalence has 2 reduction formulas.
     * The one implemented here is the one which reduces it to its normal negative form.
     *
     *       (1)                (1)
     *        |                  |
     *      (\iff)      ->     (\vee)
     *      /   \              /    \
     *     (2)  (3)           /      \
     *                   (\wedge)   (\wedge)
     *                    /    \     /      \
     *                  (2)    (3)  (\neg)  (\neg)
     *                                |       |
     *                               (2)     (3)
     */

    /*
     * Another yucky implementation. Please look into beautifying this.
     *
     * WARNING: if programs break during or after simplifications of equivalences,
     *          it may mean the pointers don't play nice with each other. Check
     *          how the spawned children nodes inherit and instead of assigning,
     *          have them copy the children of the input node.
     */

    void EquivalenceReduction::replace(const NodeObsPtr target) {
        const auto node = target.lock();

        const auto newNode = NodeBuilder::makeNewOrNode();

        // left branch
        newNode->spawn_new_child();
        newNode->children[0] = NodeBuilder::makeNewAndNode();
        newNode->children[0]->spawn_new_child();
        newNode->children[0]->children[0] = node->children[0];
        newNode->children[0]->spawn_new_child();
        newNode->children[0]->children[1] = node->children[1];

        // right branch
        newNode->spawn_new_child();
        newNode->children[1] = NodeBuilder::makeNewAndNode();
        newNode->children[1]->spawn_new_child();
        newNode->children[1]->children[0] = NodeBuilder::makeNewNotNode();
        newNode->children[1]->children[0]->spawn_new_child();
        newNode->children[1]->children[0]->children[0] = node->children[0];
        newNode->children[1]->spawn_new_child();
        newNode->children[1]->children[1] = NodeBuilder::makeNewNotNode();
        newNode->children[1]->children[1]->spawn_new_child();
        newNode->children[1]->children[1]->children[0] = node->children[1];

        // give the original node the attributes of the created node
        node->copy_from(newNode);
        node->copy_children(newNode);

    }
}