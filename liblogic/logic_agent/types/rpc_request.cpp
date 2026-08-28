//
// Created by Bogdan Petru on 05/08/2026.
//

#include "rpc_request.hpp"

#include "../rpc/rpc_exception.hpp"

using namespace logic_agent::rpc::error_code;
using logic_agent::rpc::rpc_exception;

rpc_request rpc_request::from_json(const nlohmann::json &j) {
    if (!j.is_object()) {
        throw rpc_exception(invalid_request, "Request must be a JSON object");
    }

    rpc_request request;
    request.jsonrpc = j.value("jsonrpc", std::string("2.0"));
    if (request.jsonrpc != "2.0") {
        throw rpc_exception(invalid_request, R"("jsonrpc" must be "2.0")");
    }

    if (!j.contains("method") || !j.at("method").is_string()) {
        throw rpc_exception(invalid_request,
                            R"(Request is missing a string "method")");
    }
    request.method = j.at("method").get<std::string>();

    request.params = j.value("params", nlohmann::json::object());
    request.id = j.value("id", nlohmann::json(nullptr));

    return request;
}
