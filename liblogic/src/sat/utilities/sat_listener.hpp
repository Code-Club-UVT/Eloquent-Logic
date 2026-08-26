//
// Created by Bogdan Petru on 13/08/2026.
//

#ifndef ELOQUENTLOGIC_SAT_LISTENER_HPP
#define ELOQUENTLOGIC_SAT_LISTENER_HPP
#include "common.h"
#include "feedback_listener.hpp"

namespace eloquent {
namespace logic {
class sat_listener : public feedback_listener {
  public:
    // --- Problem setup --------------------------------------------------
    CB_FULL(didBuildHeuristicsDatabase, (const ClauseSet &))

    // --- Unit propagation (one-literal-clause rule) ----------------------
    CB_FULL(didFindUnitLiteral, (Literal))
    CB_FULL(didSatisfyClauseByUnitLiteral, (const Clause &, Literal))
    CB_FULL(didReduceClauseByUnitLiteral,
            (const Clause &, const Clause &, Literal))
    CB_FULL(didDeriveEmptyClauseFromUnitPropagation, (Literal))

    // --- Pure-literal elimination (single-polarity rule) -----------------
    CB_FULL(didFindPureLiteral, (Literal))
    CB_FULL(didEliminateClauseByPureLiteral, (const Clause &, Literal))

    // --- Resolution rule ---------------------------------------------------
    CB_FULL(didSelectResolutionCandidates, (const Clause &, const Clause &))
    CB_FULL(didComputeResolvent,
            (const Clause &, const Clause &, Literal, const Clause &))
    CB_FULL(didDeriveEmptyClauseFromResolution, ())
    CB_FULL(didSkipDuplicateResolvent, (const Clause &))
    CB_FULL(didAddResolvent, (const Clause &))
    CB_FULL(didReachResolutionSaturation, ())
    CB_FULL(didExceedClauseThreshold, (size_t))

    // --- Branching / search (DPLL family) -----------------------------------
    CB_FULL(didChooseBranchingLiteral, (Literal, int))
    CB_FULL(didChooseRandomLiteral, (Literal))
    CB_FULL(didEnterBranch, (Literal, bool))
    CB_FULL(didPushBranchContext, (Literal, bool))
    CB_FULL(didPopBranchContext, ())
    CB_FULL(didFindSatisfyingAssignment, ())
    CB_FULL(didDetectConflict, ())
    CB_FULL(didBacktrack, ())

    // --- Final outcome (whole-problem determination) ------------------------
    CB_FULL(didConcludeSat, ())
    CB_FULL(didConcludeUnsat, ())
    CB_FULL(didConcludeUnknown, ())
};
} // namespace logic
} // namespace eloquent

#endif // ELOQUENTLOGIC_SAT_LISTENER_HPP
