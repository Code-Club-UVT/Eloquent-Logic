//
// Created by xcell on 19.12.2024.
//

#include "inverter.h"

namespace eloquent::logic {
    bool Inverter::match(const NodeObsPtr subtree) {
        const auto node = subtree;
        return node->getType() == NodeType::NotOp && (node->childAt(0)->get() == Node || node->children[0]->text == ELOQUENT_LIBLOGIC_SYMB_CONTRADICTION);
    }

    /*
     *      (1)
     *       |                          (1)
     *     (\neg)           ->           |
     *       |                      (\top OR \bot)
     *   (\bot OR \top)
     */
    void Inverter::replace(const NodeObsPtr target) {
        const auto node = target;

        NodePtr newNode{nullptr};
        if (node->children[0]->text == ELOQUENT_LIBLOGIC_SYMB_TAUTOLOGY) { newNode = NodeBuilder::makeNewContradictionNode(); }
        else if (node->children[0]->text == ELOQUENT_LIBLOGIC_SYMB_CONTRADICTION) { newNode = NodeBuilder::makeNewTautologyNode(); }

        node->copy_from(newNode);
        node->children.clear();
    }

}