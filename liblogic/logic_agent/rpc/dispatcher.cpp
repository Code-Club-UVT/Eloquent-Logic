#include "dispatcher.hpp"

#include <string>
#include <unordered_map>

#include "rpc_exception.hpp"
#include "methods/parse_method.hpp"
#include "methods/sat_method.hpp"
#include "methods/shutdown_method.hpp"
#include "methods/transform_method.hpp"
#include "methods/truth_table_method.hpp"

namespace logic_agent::rpc
{
    namespace
    {
        using handler_t = nlohmann::json (*)(const nlohmann::json&, event_sink&);

        // terminates_server: whether dispatch() should report should_shutdown
        // for this method — true only for "shutdown", registered right here
        // alongside its handler so the registry stays the single source of
        // truth for both "what methods exist" and "which ones stop the server".
        struct registered_method
        {
            handler_t handler;
            bool terminates_server = false;
        };

        const std::unordered_map<std::string, registered_method>& method_registry()
        {
            static const std::unordered_map<std::string, registered_method> registry{
                {"truth_table", {&methods::handle_truth_table}},
                {"parse", {&methods::handle_parse}},
                {"transform", {&methods::handle_transform}},
                {"sat", {&methods::handle_sat}},
                {"shutdown", {&methods::handle_shutdown, true}},
            };
            return registry;
        }
    }

    dispatch_result dispatch(const rpc_request& request, event_sink& sink)
    {
        const auto& registry = method_registry();
        const auto it = registry.find(request.method);
        if (it == registry.end())
        {
            throw rpc_exception(error_code::method_not_found, "Unknown method: " + request.method);
        }
        return dispatch_result{it->second.handler(request.params, sink), it->second.terminates_server};
    }
}
