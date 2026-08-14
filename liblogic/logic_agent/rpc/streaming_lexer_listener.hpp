#ifndef ELOQUENTLOGIC_AGENT_STREAMING_LEXER_LISTENER_HPP
#define ELOQUENTLOGIC_AGENT_STREAMING_LEXER_LISTENER_HPP

#include <cstddef>
#include <string_view>

#include <lexeme.hpp>
#include <lexer_listener_t.hpp>

#include "event_sink.hpp"

// Streams every eloquent::logic::lexer_listener_t callback as a
// "lexer/<callbackName>" JSON-RPC notification via `sink`.
namespace logic_agent::rpc
{
    class streaming_lexer_listener : public eloquent::logic::lexer_listener_t
    {
        event_sink& sink;

    public:
        explicit streaming_lexer_listener(event_sink& sink) : sink(sink) {}

        void didStart() override;
        void didFinish() override;
        void didRecogniseLexeme(const eloquent::logic::lexeme& l) override;
        void didReadCharacter(const char& c) override;
        void didFindUnrecognisedOperator(std::string_view text, size_t position) override;
        void didFindMissingSubscript(size_t position) override;
        void didFindUnbalancedSubscript(size_t position) override;
        void didFindUnexpectedSequence(std::string_view sequence, size_t position) override;
        void didReachEof() override;
    };
}

#endif //ELOQUENTLOGIC_AGENT_STREAMING_LEXER_LISTENER_HPP
