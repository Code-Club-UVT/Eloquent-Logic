#include "sat_method.hpp"

#include <string>

#include <common.h>
#include <dp_naive.h>
#include <dpll_mcl.h>
#include <dpll_rand.h>
#include <mapping_generator.hpp>
#include <resolution_naive_first_fit.h>
#include <transformation_runner.hpp>

#include "../formula_pipeline.hpp"
#include "../rpc_exception.hpp"
#include "../streaming_mapper_listener.hpp"
#include "../streaming_sat_listener.hpp"
#include "../streaming_transformation_listener.hpp"
#include "../../types/sat/convert.hpp"

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
    }

    nlohmann::json handle_sat(const nlohmann::json& params, event_sink& sink)
    {
        auto tree = parse_formula_from_params(params, sink);

        eloquent::logic::transformation_runner runner;
        auto transform_listener = std::make_shared<streaming_transformation_listener>(sink);
        runner.to_cnf(tree, transform_listener);

        auto mapper_listener = std::make_shared<streaming_mapper_listener>(sink);
        auto mapping_result = eloquent::logic::mapping_generator::gen_map(tree, mapper_listener);

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
