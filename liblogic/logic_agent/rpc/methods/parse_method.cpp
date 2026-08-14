#include "parse_method.hpp"

#include <syntax_tree.hpp>

#include "../../types/core/convert.hpp"
#include "../formula_pipeline.hpp"

namespace logic_agent::rpc::methods
{
    nlohmann::json handle_parse(const nlohmann::json& params, event_sink& sink)
    {
        auto tree = parse_formula_from_params(params, sink);

        // to_dto(NodeObsPtr) is null-safe, so an empty tree's null rootRef()
        // just serializes as a JSON null "tree".
        nlohmann::json result;
        result["tree"] = logic_agent::types::to_dto(tree->rootRef());
        return result;
    }
}
