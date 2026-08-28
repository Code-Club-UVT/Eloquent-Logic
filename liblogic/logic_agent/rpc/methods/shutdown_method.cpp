#include "shutdown_method.hpp"

namespace logic_agent::rpc::methods {
nlohmann::json handle_shutdown(const nlohmann::json & /*params*/,
                               event_sink & /*sink*/) {
    nlohmann::json result;
    result["message"] = "shutting down";
    return result;
}
} // namespace logic_agent::rpc::methods
