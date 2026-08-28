//
// Created by Bogdan Petru on 03/08/2026.
//

#ifndef ELOQUENTLOGIC_RELAXED_PARSER_LISTENER_HPP
#define ELOQUENTLOGIC_RELAXED_PARSER_LISTENER_HPP
#include "feedback_listener.hpp"
#include "node.h"
namespace eloquent::logic {
class relaxed_parser_listener_t : public feedback_listener {
  public:
    virtual void didParseExpression() {}
    virtual void foundUnexpectedToken(const lexeme &) {}
    virtual void parenthesisClosedTooEarly() {}
    virtual void mismatchedParens(const lexeme &) {}
    virtual void didFindPureAtomInParens(const lexeme &, const lexeme &,
                                         const lexeme &) {}
    virtual void startParsingParens(const lexeme &) {}
    // Since the logical consequence (here defined as l_equi) resolves into
    // one singular truth value, allowing for multiple logical equivalences
    // might complicate the code too much
    // TODO: remove this restriction
    virtual void foundDoubleLEqui(const lexeme &) {}
    CB_FULL(didMakeNewSubtree, (const NodeObsPtr &))
    CB_FULL(didJoin, (const NodeObsPtr &, const NodeObsPtr &))
    CB(didFindLowerPrecendenceOperator)
    CB(didReadLexeme)
};
} // namespace eloquent::logic

#endif // ELOQUENTLOGIC_RELAXED_PARSER_LISTENER_HPP
