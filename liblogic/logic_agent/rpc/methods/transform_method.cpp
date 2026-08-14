#include "transform_method.hpp"

#include <string>

#include <syntax_tree.hpp>
#include <transformation_runner.hpp>

#include "../../types/core/convert.hpp"
#include "../formula_pipeline.hpp"
#include "../rpc_exception.hpp"
#include "../streaming_transformation_listener.hpp"

namespace logic_agent::rpc::methods
{
    nlohmann::json handle_transform(const nlohmann::json& params, event_sink& sink)
    {
        auto tree = parse_formula_from_params(params, sink);
        auto target = params.value("target", std::string("cnf"));

        eloquent::logic::transformation_runner runner;
        auto listener = std::make_shared<streaming_transformation_listener>(sink);

        if (target == "nnf")
        {
            runner.to_nnf(tree, listener);
        }
        else if (target == "dnf")
        {
            runner.to_dnf(tree, listener);
        }
        else if (target == "cnf")
        {
            runner.to_cnf(tree, listener);
        }
        else
        {
            throw rpc_exception(error_code::invalid_params, R"("target" must be "nnf", "cnf", or "dnf")");
        }

        nlohmann::json result;
        result["tree"] = logic_agent::types::to_dto(tree->rootRef());
        return result;
    }
}
