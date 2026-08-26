#ifndef ELOQUENTLOGIC_AGENT_STREAMING_MAPPER_LISTENER_HPP
#define ELOQUENTLOGIC_AGENT_STREAMING_MAPPER_LISTENER_HPP

#include <map>
#include <string>

#include <common.h>
#include <mapper_listener.hpp>

#include "event_sink.hpp"

// Streams every eloquent::logic::mapper_listener callback as a
// "mapper/<callbackName>" JSON-RPC notification via `sink`.
namespace logic_agent::rpc
{
    class streaming_mapper_listener : public eloquent::logic::mapper_listener
    {
        event_sink& sink;

    public:
        explicit streaming_mapper_listener(event_sink& sink) : sink(sink) {}

        void didStart() override;
        void didFinish() override;
        void did_map_literals(const std::map<std::string, Literal>& mapping) override;
        void did_map_clauses(const ClauseSet& clauses) override;

    };
}

#endif //ELOQUENTLOGIC_AGENT_STREAMING_MAPPER_LISTENER_HPP
