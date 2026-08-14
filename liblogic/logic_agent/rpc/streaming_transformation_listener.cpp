#include "streaming_transformation_listener.hpp"

#include "../types/core/convert.hpp"
#include "../types/parser/cursor_events.hpp"
#include "../types/transformations/node_transformation_events.hpp"

namespace logic_agent::rpc
{
    void streaming_transformation_listener::emit_single_node(std::string_view method, eloquent::logic::NodeObsPtr node) const
    {
        logic_agent::types::parser::single_node_event_t dto;
        dto.node = logic_agent::types::to_dto(node);
        sink.emit(method, dto);
    }

    void streaming_transformation_listener::didStart() { sink.emit_empty("transform/didStart"); }

    void streaming_transformation_listener::didFinish() { sink.emit_empty("transform/didFinish"); }

    void streaming_transformation_listener::didMatchLogicalImplication(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didMatchLogicalImplication", node); }

    void streaming_transformation_listener::didReduceLogicalImplication(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didReduceLogicalImplication", node); }

    void streaming_transformation_listener::didCondenseChild(eloquent::logic::NodeObsPtr parent, eloquent::logic::NodeObsPtr merged)
    {
        logic_agent::types::transformations::condense_child_event_t dto;
        dto.parent = logic_agent::types::to_dto(parent);
        dto.merged_child = logic_agent::types::to_dto(merged);
        sink.emit("transform/didCondenseChild", dto);
    }

    void streaming_transformation_listener::didMatchEquivalence(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didMatchEquivalence", node); }

    void streaming_transformation_listener::didReduceEquivalence(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didReduceEquivalence", node); }

    void streaming_transformation_listener::didMatchImplication(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didMatchImplication", node); }

    void streaming_transformation_listener::didReduceImplication(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didReduceImplication", node); }

    void streaming_transformation_listener::didMatchDeMorganConjunction(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didMatchDeMorganConjunction", node); }

    void streaming_transformation_listener::didApplyDeMorganConjunction(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didApplyDeMorganConjunction", node); }

    void streaming_transformation_listener::didMatchDeMorganDisjunction(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didMatchDeMorganDisjunction", node); }

    void streaming_transformation_listener::didApplyDeMorganDisjunction(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didApplyDeMorganDisjunction", node); }

    void streaming_transformation_listener::didMatchDoubleNegation(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didMatchDoubleNegation", node); }

    void streaming_transformation_listener::didEliminateDoubleNegation(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didEliminateDoubleNegation", node); }

    void streaming_transformation_listener::didMatchInverter(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didMatchInverter", node); }

    void streaming_transformation_listener::didInvertConstant(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didInvertConstant", node); }

    void streaming_transformation_listener::didMatchNeutralElement(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didMatchNeutralElement", node); }

    void streaming_transformation_listener::didCollapseToContradiction(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didCollapseToContradiction", node); }

    void streaming_transformation_listener::didCollapseToTautology(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didCollapseToTautology", node); }

    void streaming_transformation_listener::didDropNeutralElement(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didDropNeutralElement", node); }

    void streaming_transformation_listener::didMatchAbsorption(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didMatchAbsorption", node); }

    void streaming_transformation_listener::didApplyAbsorption(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didApplyAbsorption", node); }

    void streaming_transformation_listener::didMatchAndDistribution(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didMatchAndDistribution", node); }

    void streaming_transformation_listener::didDistributeAndOverOr(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didDistributeAndOverOr", node); }

    void streaming_transformation_listener::didMatchOrDistribution(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didMatchOrDistribution", node); }

    void streaming_transformation_listener::didDistributeOrOverAnd(eloquent::logic::NodeObsPtr node) { emit_single_node("transform/didDistributeOrOverAnd", node); }

    void streaming_transformation_listener::didCheckCNF(eloquent::logic::NodeObsPtr node, bool result)
    {
        logic_agent::types::transformations::normal_form_check_event_t dto;
        dto.node = logic_agent::types::to_dto(node);
        dto.is_normal_form = result;
        sink.emit("transform/didCheckCNF", dto);
    }

    void streaming_transformation_listener::didCheckDNF(eloquent::logic::NodeObsPtr node, bool result)
    {
        logic_agent::types::transformations::normal_form_check_event_t dto;
        dto.node = logic_agent::types::to_dto(node);
        dto.is_normal_form = result;
        sink.emit("transform/didCheckDNF", dto);
    }
}
