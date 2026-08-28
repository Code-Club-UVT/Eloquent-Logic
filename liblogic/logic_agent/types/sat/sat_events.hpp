#ifndef ELOQUENTLOGIC_AGENT_SAT_EVENTS_HPP
#define ELOQUENTLOGIC_AGENT_SAT_EVENTS_HPP

#include <cstddef>

#include <nlohmann/json.hpp>

#include "../feedback_base.hpp"
#include "common_t.hpp"
#include "mapper_events.hpp"

// DTOs for eloquent::logic::sat_listener (src/sat/utilities/sat_listener.hpp)
// callback parameters that aren't already covered by a sat/mapper type:
//   - didBuildHeuristicsDatabase(const ClauseSet&)                    ->
//   clause_set_event_t (mapper_events.hpp)
//   - didFindUnitLiteral(Literal) /
//   didDeriveEmptyClauseFromUnitPropagation(Literal) /
//     didFindPureLiteral(Literal) / didChooseRandomLiteral(Literal)   ->
//     literal_event_t
//   - didSatisfyClauseByUnitLiteral(Clause, Literal) /
//     didEliminateClauseByPureLiteral(Clause, Literal)                ->
//     clause_literal_event_t
//   - didReduceClauseByUnitLiteral(Clause original, Clause reduced, Literal) ->
//   clause_reduction_event_t
//   - didSelectResolutionCandidates(Clause, Clause)                   ->
//   clause_pair_event_t
//   - didComputeResolvent(Clause, Clause, Literal pivot, Clause resolvent) ->
//   resolvent_event_t
//   - didSkipDuplicateResolvent(Clause) / didAddResolvent(Clause)     ->
//   clause_event_t
//   - didExceedClauseThreshold(size_t)                                ->
//   clause_count_event_t
//   - didChooseBranchingLiteral(Literal, int frequency)               ->
//   branching_literal_event_t
//   - didEnterBranch(Literal, bool) / didPushBranchContext(Literal, bool) ->
//   branch_event_t
//   - didDeriveEmptyClauseFromResolution() / didReachResolutionSaturation() /
//     didPopBranchContext() / didFindSatisfyingAssignment() /
//     didDetectConflict() / didBacktrack() / didConcludeSat() /
//     didConcludeUnsat() / didConcludeUnknown() -> no payload
namespace logic_agent::types::sat {
struct literal_event_t : public feedback_base {
    literal_t literal = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(literal_event_t, literal);

struct clause_literal_event_t : public feedback_base {
    clause_t clause;
    literal_t literal = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(clause_literal_event_t, clause, literal);

// didReduceClauseByUnitLiteral(original, reduced, literal)
struct clause_reduction_event_t : public feedback_base {
    clause_t original;
    clause_t reduced;
    literal_t literal = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(clause_reduction_event_t, original, reduced,
                                   literal);

struct clause_pair_event_t : public feedback_base {
    clause_t first;
    clause_t second;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(clause_pair_event_t, first, second);

// didComputeResolvent(first, second, pivot, resolvent)
struct resolvent_event_t : public feedback_base {
    clause_t first;
    clause_t second;
    literal_t pivot = 0;
    clause_t resolvent;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(resolvent_event_t, first, second, pivot,
                                   resolvent);

struct clause_event_t : public feedback_base {
    clause_t clause;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(clause_event_t, clause);

struct clause_count_event_t : public feedback_base {
    std::size_t count = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(clause_count_event_t, count);

// didChooseBranchingLiteral(literal, frequency) — see
// src/sat/dpll_mcl/dpll_utils.cpp:36 for the (max, freq) call site.
struct branching_literal_event_t : public feedback_base {
    literal_t literal = 0;
    int frequency = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(branching_literal_event_t, literal,
                                   frequency);

// Shared by didEnterBranch and didPushBranchContext, which both carry
// the branching literal and the truth value it was assigned.
struct branch_event_t : public feedback_base {
    literal_t literal = 0;
    bool value = false;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(branch_event_t, literal, value);
} // namespace logic_agent::types::sat

#endif // ELOQUENTLOGIC_AGENT_SAT_EVENTS_HPP
