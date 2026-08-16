//
// Created by xcell on 19.12.2024.
//

#include "inverter.h"

namespace eloquent::logic {
    bool Inverter::match(const NodeObsPtr subtree, const std::shared_ptr<node_transformation_listener_t>& listener) {
        bool result = subtree->getType() == NodeType::NotOp && (subtree->childAt(0)->getLexeme().type() == lexeme_type::Tautology ||
                                                         subtree->childAt(0)->getLexeme().type() == lexeme_type::Contradiction);
        if (result) listener->didMatchInverter(subtree);
        return result;
    }

    /*
     *      (1)
     *       |                          (1)
     *     (\neg)           ->           |
     *       |                      (\top OR \bot)
     *   (\bot OR \top)
     */
    void Inverter::replace(const NodeObsPtr target, const std::shared_ptr<node_transformation_listener_t>& listener) {
        auto child_lexeme = target->childAt(0)->getLexeme();
        auto child_type = child_lexeme.type();
        if (child_type == lexeme_type::Tautology)
            target->set_lexeme(lexeme::make(lexeme_type::Contradiction, symbols::SYMB_CONTRADICTION,child_lexeme.start(), child_lexeme.end()));
        else
            target->set_lexeme(lexeme::make(lexeme_type::Tautology, symbols::SYMB_TAUTOLOGY,child_lexeme.start(), child_lexeme.end()));

        (void) target->disconnect(0);

        listener->didInvertConstant(target);
    }

}