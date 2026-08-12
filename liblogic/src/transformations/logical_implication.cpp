//
// Created by Bogdan Petru on 10/08/2026.
//

#include "logical_implication.hpp"

namespace eloquent
{
    namespace logic
    {
        bool logical_implication::match(NodeObsPtr subtree)
        {
            return subtree->getType() == NodeType::LEquiOp;
        }

        void logical_implication::replace(NodeObsPtr target)
        {
            NodePtr right_subtree = target->disconnect(1);

            target->spawn_new_child(lexeme::make(lexeme_type::NotOp, symbols::SYMB_NOT, 0,0));
            target->childAt(1)->adopt(std::move(right_subtree));

            lexeme c_lexeme = target->getLexeme();
            target->set_lexeme(lexeme::make(lexeme_type::AndOp, symbols::SYMB_AND, c_lexeme.start(), c_lexeme.end()));

            target->condense();
        }
    } // logic
} // eloquent