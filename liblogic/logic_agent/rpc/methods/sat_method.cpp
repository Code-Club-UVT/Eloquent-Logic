#include "sat_method.hpp"

#include <map>
#include <string>

#include <common.h>
#include <dp_naive.h>
#include <dpll_mcl.h>
#include <dpll_rand.h>
#include <node.h>
#include <resolution_naive_first_fit.h>
#include <syntax_tree.hpp>
#include <transformation_runner.hpp>

#include "../formula_pipeline.hpp"
#include "../rpc_exception.hpp"
#include "../streaming_sat_listener.hpp"
#include "../streaming_transformation_listener.hpp"
#include "../../types/sat/convert.hpp"
#include "../../types/sat/mapper_events.hpp"

namespace logic_agent::rpc::methods
{
    namespace
    {
        const char* to_string(SatState state)
        {
            switch (state)
            {
                case SatState::SAT: return "SAT";
                case SatState::UNSAT: return "UNSAT";
                case SatState::UNKNOWN: default: return "UNKNOWN";
            }
        }

        struct mapping_result_t
        {
            std::map<std::string, Literal> mapping;
            ClauseSet clauses;
        };

        // A literal is a bare atom (positive) or its negation (NotOp
        // wrapping one Atom child, negative, DIMACS-style) — to_cnf's NNF
        // pass guarantees negation never appears any deeper than this.
        Literal literal_for(eloquent::logic::NodeObsPtr node, const std::map<std::string, Literal>& mapping)
        {
            if (node->getType() == eloquent::logic::NodeType::NotOp)
            {
                return -mapping.at(node->childAt(0)->getLexeme().token());
            }
            return mapping.at(node->getLexeme().token());
        }

        // Reimplements eloquent::logic::mapping_generator::gen_map's AST ->
        // ClauseSet walk (src/sat/utilities/mapping_generator.cpp) rather
        // than calling it directly:
        //  - gen_map passes a null node_transformation_listener_t into its
        //    internal cnf_recogniser check, which unconditionally
        //    dereferences it — a raw null-pointer segfault, not a catchable
        //    exception, so it would take the whole server down instead of
        //    just failing the one request.
        //  - gen_map's clause-building only ever looks up an Atom's own
        //    lexeme token in the mapping, so it throws on any clause
        //    containing a negated literal (NotOp-wrapping an Atom) — which
        //    to_cnf's NNF pass produces for essentially any formula that
        //    uses negation. literal_for above fixes that by mapping a
        //    negated atom to the negative of its literal id.
        mapping_result_t map_to_clauses(const std::shared_ptr<eloquent::logic::syntax_tree>& tree, event_sink& sink)
        {
            mapping_result_t result;

            Literal next = 1;
            tree->walk([&](eloquent::logic::NodeObsPtr node)
            {
                const auto& l = node->getLexeme();
                if (l.type() == eloquent::logic::lexeme_type::Atom && !result.mapping.contains(l.token()))
                {
                    result.mapping.try_emplace(l.token(), next++);
                }
            });

            logic_agent::types::sat::literal_mapping_event_t mapping_dto;
            mapping_dto.mapping = result.mapping; // Literal and literal_t are both std::int64_t
            sink.emit("mapper/did_map_literals", mapping_dto);

            auto clause_from = [&](eloquent::logic::NodeObsPtr clause_node)
            {
                using eloquent::logic::NodeType;
                if (clause_node->getType() != NodeType::OrOp)
                {
                    // A clause of exactly one literal has no OrOp wrapper.
                    return Clause{literal_for(clause_node, result.mapping)};
                }
                Clause c;
                clause_node->traverse_children([&](eloquent::logic::NodeObsPtr literal_node)
                {
                    c.emplace(literal_for(literal_node, result.mapping));
                });
                return c;
            };

            auto* root = tree->rootRef();
            if (root->getType() == eloquent::logic::NodeType::AndOp)
            {
                root->traverse_children([&](eloquent::logic::NodeObsPtr clause_node)
                {
                    result.clauses.emplace(clause_from(clause_node));
                });
            }
            else
            {
                // A formula of exactly one clause has no AndOp wrapper.
                result.clauses.emplace(clause_from(root));
            }

            logic_agent::types::sat::clause_set_event_t clauses_dto;
            clauses_dto.clauses = logic_agent::types::sat::to_dto(result.clauses);
            sink.emit("mapper/did_map_clauses", clauses_dto);

            return result;
        }
    }

    nlohmann::json handle_sat(const nlohmann::json& params, event_sink& sink)
    {
        auto tree = parse_formula_from_params(params, sink);

        eloquent::logic::transformation_runner runner;
        auto transform_listener = std::make_shared<streaming_transformation_listener>(sink);
        runner.to_cnf(tree, transform_listener);

        auto mapping_result = map_to_clauses(tree, sink);

        auto solve_listener = std::make_shared<streaming_sat_listener>(sink);
        auto algorithm = params.value("algorithm", std::string("dpll_mcl"));

        SatState state;
        if (algorithm == "dp")
        {
            state = dp(mapping_result.clauses, solve_listener);
        }
        else if (algorithm == "dpll_mcl")
        {
            state = dpll_mcl(mapping_result.clauses, solve_listener);
        }
        else if (algorithm == "dpll_rand")
        {
            state = dpll_rand(mapping_result.clauses, solve_listener);
        }
        else if (algorithm == "resolution")
        {
            state = res(mapping_result.clauses, solve_listener);
        }
        else
        {
            throw rpc_exception(error_code::invalid_params, R"("algorithm" must be "dp", "dpll_mcl", "dpll_rand", or "resolution")");
        }

        nlohmann::json result;
        result["state"] = to_string(state);
        result["mapping"] = mapping_result.mapping;
        result["clauses"] = logic_agent::types::sat::to_dto(mapping_result.clauses);
        return result;
    }
}
