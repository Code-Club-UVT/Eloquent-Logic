#include "formula_pipeline.hpp"

#include <lexer.hpp>
#include <relaxed_parser.hpp>
#include <strict_parser.hpp>

#include "rpc_exception.hpp"
#include "streaming_lexer_listener.hpp"
#include "streaming_relaxed_parser_listener.hpp"
#include "streaming_strict_parser_listener.hpp"

namespace logic_agent::rpc
{
    std::shared_ptr<eloquent::logic::syntax_tree> parse_formula(const std::string& formula, bool strict, event_sink& sink)
    {
        auto lexer_listener = std::make_shared<streaming_lexer_listener>(sink);
        auto lexemes = eloquent::logic::lexer::lex(formula, lexer_listener);

        if (strict)
        {
            auto parser_listener = std::make_shared<streaming_strict_parser_listener>(sink);
            return eloquent::logic::strict_parser::parse(lexemes, parser_listener);
        }

        auto parser_listener = std::make_shared<streaming_relaxed_parser_listener>(sink);
        return eloquent::logic::relaxed_parser::parse(lexemes, parser_listener);
    }

    std::shared_ptr<eloquent::logic::syntax_tree> parse_formula_from_params(const nlohmann::json& params, event_sink& sink)
    {
        if (!params.is_object() || !params.contains("formula") || !params.at("formula").is_string())
        {
            throw rpc_exception(error_code::invalid_params, R"(params is missing a string "formula")");
        }
        auto formula = params.at("formula").get<std::string>();
        bool strict = params.value("strict", false);
        return parse_formula(formula, strict, sink);
    }
}
