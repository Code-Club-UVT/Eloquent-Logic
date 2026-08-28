#ifndef ELOQUENTLOGIC_AGENT_STREAMING_SAT_LISTENER_HPP
#define ELOQUENTLOGIC_AGENT_STREAMING_SAT_LISTENER_HPP

#include <cstddef>

#include <common.h>
#include <sat_listener.hpp>

#include "event_sink.hpp"

// Streams every eloquent::logic::sat_listener callback as a
// "sat/<callbackName>" JSON-RPC notification via `sink`.
namespace logic_agent::rpc {
class streaming_sat_listener : public eloquent::logic::sat_listener {
    event_sink &sink;

  public:
    explicit streaming_sat_listener(event_sink &sink) : sink(sink) {}

    void didStart() override;
    void didFinish() override;

    void didBuildHeuristicsDatabase(const ClauseSet &clauses) override;

    void didFindUnitLiteral(Literal l) override;
    void didSatisfyClauseByUnitLiteral(const Clause &clause,
                                       Literal l) override;
    void didReduceClauseByUnitLiteral(const Clause &original,
                                      const Clause &reduced,
                                      Literal l) override;
    void didDeriveEmptyClauseFromUnitPropagation(Literal l) override;

    void didFindPureLiteral(Literal l) override;
    void didEliminateClauseByPureLiteral(const Clause &clause,
                                         Literal l) override;

    void didSelectResolutionCandidates(const Clause &first,
                                       const Clause &second) override;
    void didComputeResolvent(const Clause &first, const Clause &second,
                             Literal pivot, const Clause &resolvent) override;
    void didDeriveEmptyClauseFromResolution() override;
    void didSkipDuplicateResolvent(const Clause &clause) override;
    void didAddResolvent(const Clause &clause) override;
    void didReachResolutionSaturation() override;
    void didExceedClauseThreshold(size_t count) override;

    void didChooseBranchingLiteral(Literal l, int frequency) override;
    void didChooseRandomLiteral(Literal l) override;
    void didEnterBranch(Literal l, bool value) override;
    void didPushBranchContext(Literal l, bool value) override;
    void didPopBranchContext() override;
    void didFindSatisfyingAssignment() override;
    void didDetectConflict() override;
    void didBacktrack() override;

    void didConcludeSat() override;
    void didConcludeUnsat() override;
    void didConcludeUnknown() override;
};
} // namespace logic_agent::rpc

#endif // ELOQUENTLOGIC_AGENT_STREAMING_SAT_LISTENER_HPP
