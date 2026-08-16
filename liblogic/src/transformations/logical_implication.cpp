//
// Created by Bogdan Petru on 10/08/2026.
//

#include "logical_implication.hpp"

namespace eloquent
{
    namespace logic
    {
        bool logical_implication::match(NodeObsPtr subtree, const std::shared_ptr<node_transformation_listener_t>& listener)
        {
            bool result = subtree->getType() == NodeType::LEquiOp;
            if (result) listener->didMatchLogicalImplication(subtree);
            return result;
        }

        void logical_implication::replace(NodeObsPtr target, const std::shared_ptr<node_transformation_listener_t>& listener)
        {
            NodePtr right_subtree = target->disconnect(1);
            listener->didDisconnect(target, 0);

            target->spawn_new_child(lexeme::make(lexeme_type::NotOp, symbols::SYMB_NOT, 0,0));
            listener->didSpawnNewSubtree(target->lastChild());
            listener->didAdoptNode(target, target->lastChild());


            target->childAt(1)->adopt(std::move(right_subtree));
            listener->didAdoptNode(target->childAt(1), target->childAt(1)->lastChild());

            lexeme c_lexeme = target->getLexeme();
            target->set_lexeme(lexeme::make(lexeme_type::AndOp, symbols::SYMB_AND, c_lexeme.start(), c_lexeme.end()));
            listener->didTransformNodeWithLexeme(target, target->getLexeme());

            target->condense([&](NodeObsPtr parent, NodeObsPtr merged) { listener->didCondenseChild(parent, merged); });

            listener->didReduceLogicalImplication(target);
        }
    } // logic
} // eloquent