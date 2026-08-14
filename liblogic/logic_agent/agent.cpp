//
// Created by vscode on 7/30/26.
//

/**
 * A small, single-threaded JSON-RPC 2.0 server: reads Request objects from
 * STDIN one at a time, drives the requested liblogic pipeline, streams every
 * listener callback it fires as a JSON-RPC 2.0 Notification on STDOUT as it
 * happens, and finishes with a Response (result or error) carrying the
 * original id. See logic_agent/rpc/dispatcher.hpp for the method registry.
 */
#include <iostream>
#include <limits>

#include <nlohmann/json.hpp>

#include "rpc/dispatcher.hpp"
#include "rpc/event_sink.hpp"
#include "rpc/rpc_exception.hpp"
#include "types/rpc_request.hpp"
#include "types/rpc_response.hpp"

int main()
{
    // Skip whitespace first (including the trailing newline left after the
    // previous iteration's read) so peek() below sees a true EOF once STDIN
    // is genuinely exhausted, rather than one more (empty) round.
    while (std::cin >> std::ws && std::cin.peek() != std::char_traits<char>::eof())
    {
        nlohmann::json input;
        nlohmann::json id = nullptr;

        try
        {
            // nlohmann's operator>> throws json::parse_error (rather than
            // just failing the stream) on malformed input, so this has to
            // be inside the try block too, not just the dispatch below.
            std::cin >> input;
            if (input.is_object() && input.contains("id"))
            {
                id = input.at("id");
            }

            auto request = rpc_request::from_json(input);
            id = request.id;

            logic_agent::rpc::event_sink sink(std::cout);
            auto result = logic_agent::rpc::dispatch(request, sink);

            std::cout << rpc_response::success(request.id, std::move(result)).to_json().dump() << "\n";
        }
        catch (const nlohmann::json::parse_error& e)
        {
            std::cout << rpc_response::failure(id, logic_agent::rpc::error_code::parse_error, e.what()).to_json().dump() << "\n";
            // Give up on whatever's left of this line rather than retrying
            // byte-by-byte against the same garbage (which would otherwise
            // emit one parse_error per resync attempt).
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        catch (const logic_agent::rpc::rpc_exception& e)
        {
            std::cout << rpc_response::failure(id, e.code(), e.what()).to_json().dump() << "\n";
        }
        catch (const std::exception& e)
        {
            std::cout << rpc_response::failure(id, logic_agent::rpc::error_code::server_error, e.what()).to_json().dump() << "\n";
        }

        std::cout.flush();
    }

    return 0;
}
