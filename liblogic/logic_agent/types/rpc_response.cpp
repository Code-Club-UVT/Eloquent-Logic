//
// Created by Bogdan Petru on 05/08/2026.
//

#include "rpc_response.hpp"

rpc_response rpc_response::success(nlohmann::json id, nlohmann::json result)
{
    rpc_response response;
    response.id = std::move(id);
    response.result = std::move(result);
    return response;
}

rpc_response rpc_response::failure(nlohmann::json id, int code, std::string message)
{
    rpc_response response;
    response.id = std::move(id);
    response.error = nlohmann::json{
        {"code", code},
        {"message", std::move(message)},
    };
    return response;
}

nlohmann::json rpc_response::to_json() const
{
    nlohmann::json j{
        {"jsonrpc", jsonrpc},
        {"id", id},
    };
    if (!error.is_null())
    {
        j["error"] = error;
    }
    else
    {
        j["result"] = result;
    }
    return j;
}
