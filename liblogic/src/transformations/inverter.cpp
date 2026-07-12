//
// Created by xcell on 19.12.2024.
//

#include <liblogic/private/transformations/inverter.h>

namespace eloquent::logic {
    bool Inverter::match(const NodeObsPtr subtree) {
        const auto node = subtree.lock();
        return node->type == NodeType::NotOp && (node->children[0]->text == ELOQUENT_LIBLOGIC_SYMB_TAUTOLOGY || node->children[0]->text == ELOQUENT_LIBLOGIC_SYMB_CONTRADICTION);
    }

    /*
     *      (1)
     *       |                          (1)
     *     (\neg)           ->           |
     *       |                      (\top OR \bot)
     *   (\bot OR \top)
     */
    void Inverter::replace(const NodeObsPtr target) {
        const auto node = target.lock();

        NodePtr newNode{nullptr};
        if (node->children[0]->text == ELOQUENT_LIBLOGIC_SYMB_TAUTOLOGY) { newNode = NodeBuilder::makeNewContradictionNode(); }
        else if (node->children[0]->text == ELOQUENT_LIBLOGIC_SYMB_CONTRADICTION) { newNode = NodeBuilder::makeNewTautologyNode(); }

        node->copy_from(newNode);
        node->children.clear();
    }

}