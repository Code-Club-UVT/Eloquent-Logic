#include "dispatcher.hpp"

#include <string>
#include <unordered_map>

#include "rpc_exception.hpp"
#include "methods/parse_method.hpp"
#include "methods/sat_method.hpp"
#include "methods/transform_method.hpp"
#include "methods/truth_table_method.hpp"

namespace logic_agent::rpc
{
    namespace
    {
        using handler_t = nlohmann::json (*)(const nlohmann::json&, event_sink&);

        const std::unordered_map<std::string, handler_t>& method_registry()
        {
            static const std::unordered_map<std::string, handler_t> registry{
                {"truth_table", &methods::handle_truth_table},
                {"parse", &methods::handle_parse},
                {"transform", &methods::handle_transform},
                {"sat", &methods::handle_sat},
            };
            return registry;
        }
    }

    nlohmann::json dispatch(const rpc_request& request, event_sink& sink)
    {
        const auto& registry = method_registry();
        const auto it = registry.find(request.method);
        if (it == registry.end())
        {
            throw rpc_exception(error_code::method_not_found, "Unknown method: " + request.method);
        }
        return it->second(request.params, sink);
    }
}
