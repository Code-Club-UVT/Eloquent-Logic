//
// Created by Bogdan Petru on 03/08/2026.
//

#ifndef ELOQUENTLOGIC_RELAXED_PARSER_LISTENER_HPP
#define ELOQUENTLOGIC_RELAXED_PARSER_LISTENER_HPP
#include "feedback_listener.hpp"



namespace eloquent::logic
{
    class relaxed_parser_listener_t : public feedback_listener
    {
    public:
       virtual void didParseExpression() {}
       virtual void foundUnexpectedToken(const lexeme&) {}
       virtual void parenthesisClosedTooEarly() {}
       virtual void mismatchedParens(const lexeme&) {}
       virtual void didFindPureAtomInParens(const lexeme&, const lexeme&, const lexeme&) {}
       virtual void startParsingParens(const lexeme&) {}
       CB_FULL(didMakeNewSubtree, (const NodeObsPtr&))
       CB_FULL(didJoin, (const NodeObsPtr&, const NodeObsPtr&))
       CB(didFindLowerPrecendenceOperator)
       CB(didReadLexeme)
    };
}

#endif //ELOQUENTLOGIC_RELAXED_PARSER_LISTENER_HPP
