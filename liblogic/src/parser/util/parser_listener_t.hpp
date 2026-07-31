//
// Created by vscode on 7/31/26.
//

#ifndef ELOQUENTLOGIC_PARSER_LISTENER_T_HPP
#define ELOQUENTLOGIC_PARSER_LISTENER_T_HPP

#include "feedback_listener.hpp"
#include "lexeme.hpp"

namespace eloquent::logic
{
    class parser_listener_t : public feedback_listener
    {
    public:
        virtual void didProcessLexeme(const lexeme& value) {}
    };
}


#endif //ELOQUENTLOGIC_PARSER_LISTENER_T_HPP
