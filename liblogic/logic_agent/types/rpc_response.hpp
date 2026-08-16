//
// Created by Bogdan Petru on 05/08/2026.
//

#ifndef ELOQUENTLOGIC_RPC_RESPONSE_HPP
#define ELOQUENTLOGIC_RPC_RESPONSE_HPP

#include <string>

#include <nlohmann/json.hpp>

// A JSON-RPC 2.0 Response object written to STDOUT once a request finishes:
//   {"jsonrpc":"2.0","result":{...},"id":1}
// or, on failure:
//   {"jsonrpc":"2.0","error":{"code":...,"message":"..."},"id":1}
// Exactly one of result/error is ever present, so this is hand-serialized
// (via to_json()) rather than NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE, which has
// no way to omit a field conditionally.
class rpc_response
{
public:
    std::string jsonrpc = "2.0";
    nlohmann::json id = nullptr;
    nlohmann::json result = nullptr;
    nlohmann::json error = nullptr;

    static rpc_response success(nlohmann::json id, nlohmann::json result);
    static rpc_response failure(nlohmann::json id, int code, std::string message);

    [[nodiscard]] nlohmann::json to_json() const;
};

#endif //ELOQUENTLOGIC_RPC_RESPONSE_HPP
