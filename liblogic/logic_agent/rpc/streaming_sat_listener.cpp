#include "streaming_sat_listener.hpp"

#include "../types/sat/convert.hpp"
#include "../types/sat/mapper_events.hpp"
#include "../types/sat/sat_events.hpp"

namespace logic_agent::rpc {
void streaming_sat_listener::didStart() { sink.emit_empty("sat/didStart"); }

void streaming_sat_listener::didFinish() { sink.emit_empty("sat/didFinish"); }

void streaming_sat_listener::didBuildHeuristicsDatabase(
    const ClauseSet &clauses) {
    logic_agent::types::sat::clause_set_event_t dto;
    dto.clauses = logic_agent::types::sat::to_dto(clauses);
    sink.emit("sat/didBuildHeuristicsDatabase", dto);
}

void streaming_sat_listener::didFindUnitLiteral(Literal l) {
    logic_agent::types::sat::literal_event_t dto;
    dto.literal = l;
    sink.emit("sat/didFindUnitLiteral", dto);
}

void streaming_sat_listener::didSatisfyClauseByUnitLiteral(const Clause &clause,
                                                           Literal l) {
    logic_agent::types::sat::clause_literal_event_t dto;
    dto.clause = logic_agent::types::sat::to_dto(clause);
    dto.literal = l;
    sink.emit("sat/didSatisfyClauseByUnitLiteral", dto);
}

void streaming_sat_listener::didReduceClauseByUnitLiteral(
    const Clause &original, const Clause &reduced, Literal l) {
    logic_agent::types::sat::clause_reduction_event_t dto;
    dto.original = logic_agent::types::sat::to_dto(original);
    dto.reduced = logic_agent::types::sat::to_dto(reduced);
    dto.literal = l;
    sink.emit("sat/didReduceClauseByUnitLiteral", dto);
}

void streaming_sat_listener::didDeriveEmptyClauseFromUnitPropagation(
    Literal l) {
    logic_agent::types::sat::literal_event_t dto;
    dto.literal = l;
    sink.emit("sat/didDeriveEmptyClauseFromUnitPropagation", dto);
}

void streaming_sat_listener::didFindPureLiteral(Literal l) {
    logic_agent::types::sat::literal_event_t dto;
    dto.literal = l;
    sink.emit("sat/didFindPureLiteral", dto);
}

void streaming_sat_listener::didEliminateClauseByPureLiteral(
    const Clause &clause, Literal l) {
    logic_agent::types::sat::clause_literal_event_t dto;
    dto.clause = logic_agent::types::sat::to_dto(clause);
    dto.literal = l;
    sink.emit("sat/didEliminateClauseByPureLiteral", dto);
}

void streaming_sat_listener::didSelectResolutionCandidates(
    const Clause &first, const Clause &second) {
    logic_agent::types::sat::clause_pair_event_t dto;
    dto.first = logic_agent::types::sat::to_dto(first);
    dto.second = logic_agent::types::sat::to_dto(second);
    sink.emit("sat/didSelectResolutionCandidates", dto);
}

void streaming_sat_listener::didComputeResolvent(const Clause &first,
                                                 const Clause &second,
                                                 Literal pivot,
                                                 const Clause &resolvent) {
    logic_agent::types::sat::resolvent_event_t dto;
    dto.first = logic_agent::types::sat::to_dto(first);
    dto.second = logic_agent::types::sat::to_dto(second);
    dto.pivot = pivot;
    dto.resolvent = logic_agent::types::sat::to_dto(resolvent);
    sink.emit("sat/didComputeResolvent", dto);
}

void streaming_sat_listener::didDeriveEmptyClauseFromResolution() {
    sink.emit_empty("sat/didDeriveEmptyClauseFromResolution");
}

void streaming_sat_listener::didSkipDuplicateResolvent(const Clause &clause) {
    logic_agent::types::sat::clause_event_t dto;
    dto.clause = logic_agent::types::sat::to_dto(clause);
    sink.emit("sat/didSkipDuplicateResolvent", dto);
}

void streaming_sat_listener::didAddResolvent(const Clause &clause) {
    logic_agent::types::sat::clause_event_t dto;
    dto.clause = logic_agent::types::sat::to_dto(clause);
    sink.emit("sat/didAddResolvent", dto);
}

void streaming_sat_listener::didReachResolutionSaturation() {
    sink.emit_empty("sat/didReachResolutionSaturation");
}

void streaming_sat_listener::didExceedClauseThreshold(size_t count) {
    logic_agent::types::sat::clause_count_event_t dto;
    dto.count = count;
    sink.emit("sat/didExceedClauseThreshold", dto);
}

void streaming_sat_listener::didChooseBranchingLiteral(Literal l,
                                                       int frequency) {
    logic_agent::types::sat::branching_literal_event_t dto;
    dto.literal = l;
    dto.frequency = frequency;
    sink.emit("sat/didChooseBranchingLiteral", dto);
}

void streaming_sat_listener::didChooseRandomLiteral(Literal l) {
    logic_agent::types::sat::literal_event_t dto;
    dto.literal = l;
    sink.emit("sat/didChooseRandomLiteral", dto);
}

void streaming_sat_listener::didEnterBranch(Literal l, bool value) {
    logic_agent::types::sat::branch_event_t dto;
    dto.literal = l;
    dto.value = value;
    sink.emit("sat/didEnterBranch", dto);
}

void streaming_sat_listener::didPushBranchContext(Literal l, bool value) {
    logic_agent::types::sat::branch_event_t dto;
    dto.literal = l;
    dto.value = value;
    sink.emit("sat/didPushBranchContext", dto);
}

void streaming_sat_listener::didPopBranchContext() {
    sink.emit_empty("sat/didPopBranchContext");
}

void streaming_sat_listener::didFindSatisfyingAssignment() {
    sink.emit_empty("sat/didFindSatisfyingAssignment");
}

void streaming_sat_listener::didDetectConflict() {
    sink.emit_empty("sat/didDetectConflict");
}

void streaming_sat_listener::didBacktrack() {
    sink.emit_empty("sat/didBacktrack");
}

void streaming_sat_listener::didConcludeSat() {
    sink.emit_empty("sat/didConcludeSat");
}

void streaming_sat_listener::didConcludeUnsat() {
    sink.emit_empty("sat/didConcludeUnsat");
}

void streaming_sat_listener::didConcludeUnknown() {
    sink.emit_empty("sat/didConcludeUnknown");
}
} // namespace logic_agent::rpc
