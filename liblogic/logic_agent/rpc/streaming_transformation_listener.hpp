#ifndef ELOQUENTLOGIC_AGENT_STREAMING_TRANSFORMATION_LISTENER_HPP
#define ELOQUENTLOGIC_AGENT_STREAMING_TRANSFORMATION_LISTENER_HPP

#include <string_view>

#include <node.h>
#include <node_transformation_listener_t.hpp>

#include "event_sink.hpp"

// Streams every eloquent::logic::node_transformation_listener_t callback as
// a "transform/<callbackName>" JSON-RPC notification via `sink`.
namespace logic_agent::rpc {
class streaming_transformation_listener
    : public eloquent::logic::node_transformation_listener_t {
    event_sink &sink;

    // Shared by every callback below that carries exactly one Node* —
    // the vast majority of node_transformation_listener_t's callbacks.
    void emit_single_node(std::string_view method,
                          eloquent::logic::NodeObsPtr node) const;

  public:
    explicit streaming_transformation_listener(event_sink &sink) : sink(sink) {}

    void didStart() override;
    void didFinish() override;

    void didSpawnNewSubtree(eloquent::logic::NodeObsPtr node) override;
    void didAdoptNode(eloquent::logic::NodeObsPtr parent,
                      eloquent::logic::NodeObsPtr child) override;
    void didDisconnect(eloquent::logic::NodeObsPtr node, size_t index) override;
    void didTransformNodeWithLexeme(eloquent::logic::NodeObsPtr node,
                                    eloquent::logic::lexeme lexeme) override;
    void didTransferNode(eloquent::logic::NodeObsPtr source, size_t index,
                         eloquent::logic::NodeObsPtr destination) override;
    void didDiscardAllChildren(eloquent::logic::NodeObsPtr node) override;

    void didMatchLogicalImplication(eloquent::logic::NodeObsPtr node) override;
    void didReduceLogicalImplication(eloquent::logic::NodeObsPtr node) override;
    void didCondenseChild(eloquent::logic::NodeObsPtr parent,
                          eloquent::logic::NodeObsPtr merged) override;
    void didMatchEquivalence(eloquent::logic::NodeObsPtr node) override;
    void didReduceEquivalence(eloquent::logic::NodeObsPtr node) override;
    void didMatchImplication(eloquent::logic::NodeObsPtr node) override;
    void didReduceImplication(eloquent::logic::NodeObsPtr node) override;
    void didMatchDeMorganConjunction(eloquent::logic::NodeObsPtr node) override;
    void didApplyDeMorganConjunction(eloquent::logic::NodeObsPtr node) override;
    void didMatchDeMorganDisjunction(eloquent::logic::NodeObsPtr node) override;
    void didApplyDeMorganDisjunction(eloquent::logic::NodeObsPtr node) override;
    void didMatchDoubleNegation(eloquent::logic::NodeObsPtr node) override;
    void didEliminateDoubleNegation(eloquent::logic::NodeObsPtr node) override;
    void didMatchInverter(eloquent::logic::NodeObsPtr node) override;
    void didInvertConstant(eloquent::logic::NodeObsPtr node) override;
    void didMatchNeutralElement(eloquent::logic::NodeObsPtr node) override;
    void didCollapseToContradiction(eloquent::logic::NodeObsPtr node) override;
    void didCollapseToTautology(eloquent::logic::NodeObsPtr node) override;
    void didDropNeutralElement(eloquent::logic::NodeObsPtr node) override;
    void didMatchAbsorption(eloquent::logic::NodeObsPtr node) override;
    void didApplyAbsorption(eloquent::logic::NodeObsPtr node) override;
    void didMatchAndDistribution(eloquent::logic::NodeObsPtr node) override;
    void didDistributeAndOverOr(eloquent::logic::NodeObsPtr node) override;
    void didMatchOrDistribution(eloquent::logic::NodeObsPtr node) override;
    void didDistributeOrOverAnd(eloquent::logic::NodeObsPtr node) override;
    void didCheckCNF(eloquent::logic::NodeObsPtr node, bool result) override;
    void didCheckDNF(eloquent::logic::NodeObsPtr node, bool result) override;
};
} // namespace logic_agent::rpc

#endif // ELOQUENTLOGIC_AGENT_STREAMING_TRANSFORMATION_LISTENER_HPP
