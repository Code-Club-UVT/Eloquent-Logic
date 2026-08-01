//
// Created by vscode on 7/31/26.
//

#ifndef ELOQUENTLOGIC_PARSER_LISTENER_T_HPP
#define ELOQUENTLOGIC_PARSER_LISTENER_T_HPP

#include "cursor_listener_t.hpp"
#include "lexeme.hpp"
#include "node.h"
#include "strict_parser.hpp"

namespace eloquent::logic
{
    class parser_listener_t : public cursor_listener_t
    {
    public:
        virtual void didProcessLexeme(const lexeme& value) {}
        virtual void didMakeNode(const NodeObsPtr& node) {}
        virtual void didPlaceNodeAtCursor(const NodeObsPtr& node) {}
        virtual void foundUnexpectedToken(const lexeme& peek) {}
        virtual void foundMismatchedParanthesis() {}
        virtual void startedProcessingParanthesis() {}
    };
}


#endif //ELOQUENTLOGIC_PARSER_LISTENER_T_HPP
