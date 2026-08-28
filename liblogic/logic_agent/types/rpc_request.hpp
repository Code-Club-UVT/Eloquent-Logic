//
// Created by Bogdan Petru on 05/08/2026.
//

#ifndef ELOQUENTLOGIC_RPC_REQUEST_HPP
#define ELOQUENTLOGIC_RPC_REQUEST_HPP

#include <string>

#include <nlohmann/json.hpp>

// A JSON-RPC 2.0 Request object read from STDIN:
//   {"jsonrpc":"2.0","method":"truth_table","params":{"formula":"..."},"id":1}
// `params`/`id` are optional per the spec (a request without "id" is a
// notification), so this is hand-parsed with nlohmann::json::value(...)
// rather than NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE, whose generated
// from_json uses .at() and throws on any missing key.
class rpc_request {
  public:
    std::string jsonrpc = "2.0";
    std::string method;
    nlohmann::json params = nlohmann::json::object();
    nlohmann::json id = nullptr;

    // Throws logic_agent::rpc::rpc_exception(-32600, ...) (see
    // logic_agent/rpc/rpc_exception.hpp) if `j` isn't a well-formed Request
    // object (not an object, wrong/missing "jsonrpc", or missing "method").
    static rpc_request from_json(const nlohmann::json &j);
};

#endif // ELOQUENTLOGIC_RPC_REQUEST_HPP
