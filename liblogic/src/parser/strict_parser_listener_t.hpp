//
// Created by vscode on 7/31/26.
//

#ifndef ELOQUENTLOGIC_PARSER_LISTENER_T_HPP
#define ELOQUENTLOGIC_PARSER_LISTENER_T_HPP

#include "cursor.h"
#include "cursor_listener_t.hpp"
#include "lexeme.hpp"
#include "node.h"

namespace eloquent::logic {
class strict_parser_listener_t : public cursor_listener_t {
  public:
    virtual void didProcessLexeme(const lexeme &) {}
    virtual void didMakeNode(const NodeObsPtr &) {}
    virtual void didPlaceNodeAtCursor(const NodeObsPtr &) {}
    virtual void foundUnexpectedToken(const lexeme &) {}
    // Since the logical consequence (here defined as l_equi) resolves into
    // one singular truth value, allowing for multiple logical equivalences
    // might complicate the code too much
    // TODO: remove this restriction
    virtual void foundDoubleLEqui(const lexeme &) {}
    virtual void foundMismatchedParanthesis() {}
    virtual void startedProcessingParanthesis() {}
    virtual void foundAstError(const Cursor &) {}

    virtual void acceptUnaryOpVariant() {}
    virtual void acceptBinaryOpVariant() {}
    virtual void wrongArityForNode(NodeObsPtr) {}
};
} // namespace eloquent::logic

#endif // ELOQUENTLOGIC_PARSER_LISTENER_T_HPP
