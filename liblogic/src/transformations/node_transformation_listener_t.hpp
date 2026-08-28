//
// Created by Bogdan Petru on 10/08/2026.
//

#ifndef ELOQUENTLOGIC_NODE_TRANSFORMATION_LISTENER_T_HPP
#define ELOQUENTLOGIC_NODE_TRANSFORMATION_LISTENER_T_HPP

#include "feedback_listener.hpp"
#include "node.h"

namespace eloquent {
namespace logic {
class node_transformation_listener_t : public feedback_listener {
  public:
    // Node operation replication
    CB_FULL(didSpawnNewSubtree, (NodeObsPtr));
    CB_FULL(didAdoptNode, (NodeObsPtr, NodeObsPtr))
    // This signal does NOT mean that the node has been discarded. A node may
    // only be considered discarded once the transformation finalizer signal
    // fires and the node is not a child of any parent node and is not the root
    // node
    CB_FULL(didDisconnect, (NodeObsPtr, size_t))
    CB_FULL(didTransformNodeWithLexeme, (NodeObsPtr, lexeme))
    CB_FULL(didTransferNode, (NodeObsPtr, size_t, NodeObsPtr))
    CB_FULL(didDiscardAllChildren, (NodeObsPtr))

    // Implication-family reductions
    CB_FULL(didMatchLogicalImplication, (NodeObsPtr))
    CB_FULL(didReduceLogicalImplication, (NodeObsPtr))

    // n-isation (Node::condense()): flattens a nested same-type
    // AndOp/OrOp child into its parent. Fired once per merged child,
    // so a cascade of nested chains is reported step by step.
    CB_FULL(didCondenseChild, (NodeObsPtr, NodeObsPtr))

    CB_FULL(didMatchEquivalence, (NodeObsPtr))
    CB_FULL(didReduceEquivalence, (NodeObsPtr))
    CB_FULL(didMatchImplication, (NodeObsPtr))
    CB_FULL(didReduceImplication, (NodeObsPtr))

    // De Morgan's laws
    CB_FULL(didMatchDeMorganConjunction, (NodeObsPtr))
    CB_FULL(didApplyDeMorganConjunction, (NodeObsPtr))
    CB_FULL(didMatchDeMorganDisjunction, (NodeObsPtr))
    CB_FULL(didApplyDeMorganDisjunction, (NodeObsPtr))

    // Double negation / constant inversion
    CB_FULL(didMatchDoubleNegation, (NodeObsPtr))
    CB_FULL(didEliminateDoubleNegation, (NodeObsPtr))
    CB_FULL(didMatchInverter, (NodeObsPtr))
    CB_FULL(didInvertConstant, (NodeObsPtr))

    // Neutral-element simplification. replace() can cascade through
    // several of these in a single call for nested formulas.
    CB_FULL(didMatchNeutralElement, (NodeObsPtr))
    CB_FULL(didCollapseToContradiction, (NodeObsPtr))
    CB_FULL(didCollapseToTautology, (NodeObsPtr))
    CB_FULL(didDropNeutralElement, (NodeObsPtr))

    // Absorption law
    CB_FULL(didMatchAbsorption, (NodeObsPtr))
    CB_FULL(didApplyAbsorption, (NodeObsPtr))

    // Distribution (used while converting to CNF/DNF)
    CB_FULL(didMatchAndDistribution, (NodeObsPtr))
    CB_FULL(didDistributeAndOverOr, (NodeObsPtr))
    CB_FULL(didMatchOrDistribution, (NodeObsPtr))
    CB_FULL(didDistributeOrOverAnd, (NodeObsPtr))

    // Normal-form recognition (loop-termination checks). Reports
    // both outcomes, since "not yet in normal form" is itself a
    // meaningful step in the algorithm's execution.
    CB_FULL(didCheckCNF, (NodeObsPtr, bool))
    CB_FULL(didCheckDNF, (NodeObsPtr, bool))
};
} // namespace logic
} // namespace eloquent

#endif // ELOQUENTLOGIC_NODE_TRANSFORMATION_LISTENER_T_HPP
