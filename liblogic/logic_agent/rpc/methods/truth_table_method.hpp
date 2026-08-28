#ifndef ELOQUENTLOGIC_AGENT_TRUTH_TABLE_METHOD_HPP
#define ELOQUENTLOGIC_AGENT_TRUTH_TABLE_METHOD_HPP

#include <nlohmann/json.hpp>

#include "../event_sink.hpp"

// RPC method "truth_table": {formula, strict?} ->
//   {headers: [...variables, formula], rows: [[bool...]...]}
// Each row has one bool per variable (in `headers` order) plus one trailing
// bool for the whole formula's value under that assignment.
namespace logic_agent::rpc::methods {
nlohmann::json handle_truth_table(const nlohmann::json &params,
                                  event_sink &sink);
}

#endif // ELOQUENTLOGIC_AGENT_TRUTH_TABLE_METHOD_HPP
