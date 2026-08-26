#ifndef ELOQUENTLOGIC_AGENT_FORMULA_PIPELINE_HPP
#define ELOQUENTLOGIC_AGENT_FORMULA_PIPELINE_HPP

#include <memory>
#include <string>

#include <nlohmann/json.hpp>
#include <syntax_tree.hpp>

#include "event_sink.hpp"

// Shared first stage of every RPC method: lex `formula` and parse it into a
// syntax_tree, streaming every lexer/parser callback through `sink` as it
// goes. `strict` selects strict_parser (requires fully parenthesized input,
// e.g. "(A \\wedge B)", streamed under "parser/strict/*") over the default
// relaxed_parser (Pratt-style, streamed under "parser/*").
// Throws whatever lexer::lex/relaxed_parser::parse/strict_parser::parse
// throw (lexer_exception, unknown_variable_error) on malformed input.
namespace logic_agent::rpc {
std::shared_ptr<eloquent::logic::syntax_tree>
parse_formula(const std::string &formula, bool strict, event_sink &sink);

// Convenience wrapper used by every methods/*_method.cpp: pulls the
// required "formula" string and optional "strict" bool (default false)
// out of a request's `params` object (throwing rpc_exception(-32602, ...)
// if "formula" is missing/not a string), then calls parse_formula.
std::shared_ptr<eloquent::logic::syntax_tree>
parse_formula_from_params(const nlohmann::json &params, event_sink &sink);
} // namespace logic_agent::rpc

#endif // ELOQUENTLOGIC_AGENT_FORMULA_PIPELINE_HPP
