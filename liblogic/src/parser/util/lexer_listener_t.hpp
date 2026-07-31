//
// Created by vscode on 7/30/26.
//

#ifndef ELOQUENTLOGIC_LEXER_LISTENER_T_HPP
#define ELOQUENTLOGIC_LEXER_LISTENER_T_HPP
#include "feedback_listener.hpp"
#include "lexeme.hpp"

namespace eloquent::logic
{

    class lexer_listener_t : public feedback_listener
    {
        public:
        virtual void didRecogniseLexeme(const lexeme& l) {}
        virtual void didReadCharacter(const char& c) {};
        virtual void didFindUnrecognisedOperator(std::string_view s, size_t idx){};
        virtual void didFindMissingSubscript(size_t idx){}
        virtual void didFindUnbalancedSubscript(size_t idx){}
        virtual void didFindUnexpectedSequence(std::string_view s, size_t start_idx) {};
    };
}


#endif //ELOQUENTLOGIC_LEXER_LISTENER_T_HPP
