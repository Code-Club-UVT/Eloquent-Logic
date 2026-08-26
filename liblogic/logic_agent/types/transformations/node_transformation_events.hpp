#ifndef ELOQUENTLOGIC_AGENT_NODE_TRANSFORMATION_EVENTS_HPP
#define ELOQUENTLOGIC_AGENT_NODE_TRANSFORMATION_EVENTS_HPP

#include <cstddef>
#include <optional>

#include <nlohmann/json.hpp>

#include "../core/lexeme_t.hpp"
#include "../core/node_t.hpp"
#include "../feedback_base.hpp"
#include "../parser/cursor_events.hpp"

// DTOs for eloquent::logic::node_transformation_listener_t
// (src/transformations/node_transformation_listener_t.hpp) callback
// parameters that aren't already covered by a core type:
//   - didSpawnNewSubtree(Node*) / didDiscardAllChildren(Node*)
//       -> logic_agent::types::parser::single_node_event_t (cursor_events.hpp)
//   - didAdoptNode(Node* parent, Node* child) -> adopt_node_event_t
//   - didDisconnect(Node* node, size_t index) -> disconnect_event_t
//   - didTransformNodeWithLexeme(Node* node, lexeme) ->
//   transform_lexeme_event_t
//   - didTransferNode(Node* source, size_t index, Node* destination) ->
//   transfer_node_event_t
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
//   - didCheckCNF(Node*, bool) / didCheckDNF(Node*, bool) ->
//   normal_form_check_event_t
namespace logic_agent::types::transformations {
// didAdoptNode(parent, child) — parent has just taken ownership of
// child as a new/relocated subtree (see src/transformations/distributions.cpp).
struct adopt_node_event_t : public feedback_base {
    std::optional<node_t> parent;
    std::optional<node_t> child;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(adopt_node_event_t, parent, child)

// didDisconnect(node, index) — the child at `index` was removed from
// `node`. This signal does NOT mean that the node has been discarded. A
// node may only be considered discarded once the transformation
// finalizer signal fires and the node is not a child of any parent node
// and is not the root node.
struct disconnect_event_t : public feedback_base {
    std::optional<node_t> node;
    size_t index = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(disconnect_event_t, node, index)

// didTransformNodeWithLexeme(node, lexeme) — node's lexeme was just set
// to `lexeme` in place (see src/transformations/inverter.cpp).
struct transform_lexeme_event_t : public feedback_base {
    std::optional<node_t> node;
    lexeme_t lexeme;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(transform_lexeme_event_t, node, lexeme)

// didTransferNode(source, index, destination) — the child of `source`
// at `index` was moved to become a child of `destination` (see
// src/transformations/double_negation.cpp).
struct transfer_node_event_t : public feedback_base {
    std::optional<node_t> source;
    size_t index = 0;
    std::optional<node_t> destination;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(transfer_node_event_t, source, index,
                                   destination)

// didCondenseChild(target, merged) — target is the node being
// flattened into, merged is the same-type child it absorbed (see
// src/transformations/logical_implication.cpp).
struct condense_child_event_t : public feedback_base {
    std::optional<node_t> parent;
    std::optional<node_t> merged_child;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(condense_child_event_t, parent, merged_child)

// Shared by didCheckCNF and didCheckDNF, which both carry the checked
// node and whether it is already in that normal form.
struct normal_form_check_event_t : public feedback_base {
    std::optional<node_t> node;
    bool is_normal_form = false;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(normal_form_check_event_t, node,
                                   is_normal_form)
} // namespace logic_agent::types::transformations

#endif // ELOQUENTLOGIC_AGENT_NODE_TRANSFORMATION_EVENTS_HPP
