#include "truth_table_method.hpp"

#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

#include <lexeme.hpp>
#include <node.h>
#include <syntax_tree.hpp>
#include <truth_table.hpp>

#include "../formula_pipeline.hpp"
#include "../streaming_truth_table_listener.hpp"

namespace logic_agent::rpc::methods {
nlohmann::json handle_truth_table(const nlohmann::json &params,
                                  event_sink &sink) {
    auto tree = parse_formula_from_params(params, sink);

    auto table = eloquent::logic::truth_table::make_from_tree(tree);
    // get_variables() reads the (unordered_map-backed) interpretation's
    // current key order; captured once here, it stays valid for every
    // row below because evaluate_all only ever *overwrites* those keys'
    // values (never inserts/removes any), so the map never rehashes.
    auto variables = table.get_variables();

    auto listener = std::make_shared<streaming_truth_table_listener>(sink);
    auto rows = table.evaluate_all(listener);

    // Mirrors truth_table::evaluate_all's own bit assignment
    // (it.try_emplace(get_variables()[j], row_index & (1<<j))): bit j of
    // the row index gives the value of variables[j] for that row.
    auto variable_value = [&](std::size_t variable_index,
                              std::size_t row_index) {
        return static_cast<bool>(
            row_index & (static_cast<std::size_t>(1) << variable_index));
    };

    // table_row only caches *computed* (operator) subexpressions, not
    // atoms/constants (see truth_table::get_value), so a formula that's
    // just a bare atom or \top/\bot has no cache entry for its root —
    // mirror get_value's own per-type logic for the root's value.
    const auto root_lexeme = tree->rootRef()->getLexeme();
    const auto root_uuid = tree->rootRef()->getUUID();
    auto root_value = [&](std::size_t row_index) -> bool {
        using eloquent::logic::lexeme_type;
        switch (root_lexeme.type()) {
        case lexeme_type::Atom: {
            const auto it = std::find(variables.begin(), variables.end(),
                                      root_lexeme.token());
            return variable_value(
                static_cast<std::size_t>(std::distance(variables.begin(), it)),
                row_index);
        }
        case lexeme_type::Tautology:
            return true;
        case lexeme_type::Contradiction:
            return false;
        default:
            return rows[row_index].at(root_uuid);
        }
    };

    nlohmann::json headers = variables;
    headers.push_back(params.at("formula").get<std::string>());

    nlohmann::json json_rows = nlohmann::json::array();
    for (std::size_t i = 0; i < rows.size(); ++i) {
        nlohmann::json json_row = nlohmann::json::array();
        for (std::size_t j = 0; j < variables.size(); ++j) {
            json_row.push_back(variable_value(j, i));
        }
        json_row.push_back(root_value(i));
        json_rows.push_back(std::move(json_row));
    }

    nlohmann::json result;
    result["headers"] = std::move(headers);
    result["rows"] = std::move(json_rows);
    return result;
}
} // namespace logic_agent::rpc::methods
