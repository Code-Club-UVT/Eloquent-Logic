#ifndef ELOQUENTLOGIC_AGENT_STREAMING_STRICT_PARSER_LISTENER_HPP
#define ELOQUENTLOGIC_AGENT_STREAMING_STRICT_PARSER_LISTENER_HPP

#include <cstddef>

#include <cursor.h>
#include <lexeme.hpp>
#include <node.h>
#include <strict_parser_listener_t.hpp>

#include "event_sink.hpp"

// Streams every eloquent::logic::strict_parser_listener_t callback — plus
// the eloquent::logic::cursor_listener_t callbacks it inherits — as a
// "parser/strict/<callbackName>" JSON-RPC notification via `sink`.
namespace logic_agent::rpc {
class streaming_strict_parser_listener
    : public eloquent::logic::strict_parser_listener_t {
    event_sink &sink;

  public:
    explicit streaming_strict_parser_listener(event_sink &sink) : sink(sink) {}

    void didStart() override;
    void didFinish() override;

    // cursor_listener_t
    void didGoUp() override;
    void didTryInvalidPosition(eloquent::logic::NodeObsPtr node,
                               size_t idx) override;
    void
    didTrySpawiningOverExistingNode(eloquent::logic::NodeObsPtr node) override;
    void spawnedNewChildNode(const eloquent::logic::NodeObsPtr &parent,
                             const eloquent::logic::NodeObsPtr &child) override;
    void didPlaceNode() override;
    void alreadyUp() override;

    // strict_parser_listener_t
    void didProcessLexeme(const eloquent::logic::lexeme &l) override;
    void didMakeNode(const eloquent::logic::NodeObsPtr &node) override;
    void didPlaceNodeAtCursor(const eloquent::logic::NodeObsPtr &node) override;
    void foundUnexpectedToken(const eloquent::logic::lexeme &l) override;
    void foundMismatchedParanthesis() override;
    void startedProcessingParanthesis() override;
    void foundAstError(const eloquent::logic::Cursor &cursor) override;
    void acceptUnaryOpVariant() override;
    void acceptBinaryOpVariant() override;
    void wrongArityForNode(eloquent::logic::NodeObsPtr node) override;
};
} // namespace logic_agent::rpc

#endif // ELOQUENTLOGIC_AGENT_STREAMING_STRICT_PARSER_LISTENER_HPP
