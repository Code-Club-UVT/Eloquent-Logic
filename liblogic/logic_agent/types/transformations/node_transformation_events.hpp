#ifndef ELOQUENTLOGIC_AGENT_NODE_TRANSFORMATION_EVENTS_HPP
#define ELOQUENTLOGIC_AGENT_NODE_TRANSFORMATION_EVENTS_HPP

#include <optional>

#include <nlohmann/json.hpp>

#include "../core/node_t.hpp"
#include "../parser/cursor_events.hpp"

// DTOs for eloquent::logic::node_transformation_listener_t
// (src/transformations/node_transformation_listener_t.hpp) callback
// parameters that aren't already covered by a core type:
//   - didMatchLogicalImplication(Node*) / didReduceLogicalImplication(Node*) /
//     didMatchEquivalence(Node*) / didReduceEquivalence(Node*) /
//     didMatchImplication(Node*) / didReduceImplication(Node*) /
//     didMatchDeMorganConjunction(Node*) / didApplyDeMorganConjunction(Node*) /
//     didMatchDeMorganDisjunction(Node*) / didApplyDeMorganDisjunction(Node*) /
//     didMatchDoubleNegation(Node*) / didEliminateDoubleNegation(Node*) /
//     didMatchInverter(Node*) / didInvertConstant(Node*) /
//     didMatchNeutralElement(Node*) / didCollapseToContradiction(Node*) /
//     didCollapseToTautology(Node*) / didDropNeutralElement(Node*) /
//     didMatchAbsorption(Node*) / didApplyAbsorption(Node*) /
//     didMatchAndDistribution(Node*) / didDistributeAndOverOr(Node*) /
//     didMatchOrDistribution(Node*) / didDistributeOrOverAnd(Node*)
//       -> logic_agent::types::parser::single_node_event_t (cursor_events.hpp)
//   - didCondenseChild(Node* parent, Node* merged)  -> condense_child_event_t
//   - didCheckCNF(Node*, bool) / didCheckDNF(Node*, bool) -> normal_form_check_event_t
namespace logic_agent::types::transformations
{
    // didCondenseChild(target, merged) — target is the node being
    // flattened into, merged is the same-type child it absorbed (see
    // src/transformations/logical_implication.cpp).
    struct condense_child_event_t
    {
        std::optional<node_t> parent;
        std::optional<node_t> merged_child;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(condense_child_event_t, parent, merged_child)

    // Shared by didCheckCNF and didCheckDNF, which both carry the checked
    // node and whether it is already in that normal form.
    struct normal_form_check_event_t
    {
        std::optional<node_t> node;
        bool is_normal_form = false;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(normal_form_check_event_t, node, is_normal_form)
}

#endif //ELOQUENTLOGIC_AGENT_NODE_TRANSFORMATION_EVENTS_HPP
