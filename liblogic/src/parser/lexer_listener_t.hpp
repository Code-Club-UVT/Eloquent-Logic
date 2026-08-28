//
// Created by vscode on 7/30/26.
//

#ifndef ELOQUENTLOGIC_LEXER_LISTENER_T_HPP
#define ELOQUENTLOGIC_LEXER_LISTENER_T_HPP
#include "feedback_listener.hpp"
#include "lexeme.hpp"

namespace eloquent::logic {

class lexer_listener_t : public feedback_listener {
  public:
    virtual void didRecogniseLexeme(const lexeme &) {}
    virtual void didReadCharacter(const char &) {};
    virtual void didFindUnrecognisedOperator(std::string_view, size_t) {};
    virtual void didFindMissingSubscript(size_t) {}
    virtual void didFindUnbalancedSubscript(size_t) {}
    virtual void didFindUnexpectedSequence(std::string_view, size_t) {};
    virtual void didReachEof() {}
};
} // namespace eloquent::logic

#endif // ELOQUENTLOGIC_LEXER_LISTENER_T_HPP
