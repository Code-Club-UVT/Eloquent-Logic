#include "logical_consequence_method.hpp"

#include <logical_consequence.hpp>

#include "../formula_pipeline.hpp"
#include "../streaming_logical_consequence_listener.hpp"
#include "../streaming_mapper_listener.hpp"
#include "../streaming_sat_listener.hpp"
#include "../streaming_transformation_listener.hpp"

namespace logic_agent::rpc::methods {
nlohmann::json handle_logical_consequence(const nlohmann::json &params,
                                          event_sink &sink) {
    auto tree = parse_formula_from_params(params, sink);

    auto consequence_listener =
        std::make_shared<streaming_logical_consequence_listener>(sink);
    auto node_listener = std::make_shared<streaming_transformation_listener>(sink);
    auto sat_listener = std::make_shared<streaming_sat_listener>(sink);
    auto mapper_listener = std::make_shared<streaming_mapper_listener>(sink);

    bool result = eloquent::logic::logical_consequence::solve_logical_consequence(
        tree, consequence_listener, node_listener, sat_listener,
        mapper_listener);

    nlohmann::json out;
    out["result"] = result;
    return out;
}
} // namespace logic_agent::rpc::methods
