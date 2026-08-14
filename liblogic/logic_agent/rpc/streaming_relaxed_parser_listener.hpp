#ifndef ELOQUENTLOGIC_AGENT_STREAMING_RELAXED_PARSER_LISTENER_HPP
#define ELOQUENTLOGIC_AGENT_STREAMING_RELAXED_PARSER_LISTENER_HPP

#include <lexeme.hpp>
#include <node.h>
#include <relaxed_parser_listener.hpp>

#include "event_sink.hpp"

// Streams every eloquent::logic::relaxed_parser_listener_t callback as a
// "parser/<callbackName>" JSON-RPC notification via `sink`.
namespace logic_agent::rpc
{
    class streaming_relaxed_parser_listener : public eloquent::logic::relaxed_parser_listener_t
    {
        event_sink& sink;

    public:
        explicit streaming_relaxed_parser_listener(event_sink& sink) : sink(sink) {}

        void didStart() override;
        void didFinish() override;
        void didParseExpression() override;
        void foundUnexpectedToken(const eloquent::logic::lexeme& l) override;
        void parenthesisClosedTooEarly() override;
        void mismatchedParens(const eloquent::logic::lexeme& l) override;
        void didFindPureAtomInParens(const eloquent::logic::lexeme& open, const eloquent::logic::lexeme& atom, const eloquent::logic::lexeme& close) override;
        void startParsingParens(const eloquent::logic::lexeme& l) override;
        void didMakeNewSubtree(const eloquent::logic::NodeObsPtr& node) override;
        void didJoin(const eloquent::logic::NodeObsPtr& target, const eloquent::logic::NodeObsPtr& source) override;
        void didFindLowerPrecendenceOperator(const eloquent::logic::lexeme& l) override;
        void didReadLexeme(const eloquent::logic::lexeme& l) override;
    };
}

#endif //ELOQUENTLOGIC_AGENT_STREAMING_RELAXED_PARSER_LISTENER_HPP
