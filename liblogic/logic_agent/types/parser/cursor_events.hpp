#ifndef ELOQUENTLOGIC_AGENT_CURSOR_EVENTS_HPP
#define ELOQUENTLOGIC_AGENT_CURSOR_EVENTS_HPP

#include <cstddef>
#include <optional>

#include <nlohmann/json.hpp>

#include "../core/node_t.hpp"

// DTOs for eloquent::logic::cursor_listener_t (src/parser/cursor_listener_t.hpp)
// callback parameters that aren't already covered by a core type:
//   - didTryInvalidPosition(Node*, size_t)          -> invalid_position_event_t
//   - didTrySpawiningOverExistingNode(Node*)         -> single_node_event_t
//   - spawnedNewChildNode(Node* parent, Node* child) -> spawned_child_event_t
//   - didGoUp() / didPlaceNode() / alreadyUp()       -> no payload
// single_node_event_t is also reused by strict_parser_listener_t's
// didMakeNode/didPlaceNodeAtCursor/wrongArityForNode and relaxed_parser_listener_t's
// didMakeNewSubtree, which all carry exactly one Node*.
namespace logic_agent::types::parser
{
    struct single_node_event_t
    {
        std::optional<node_t> node;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(single_node_event_t, node)

    // didTryInvalidPosition(Node* node, size_t idx) — idx is the
    // out-of-range child index the cursor tried to move to.
    struct invalid_position_event_t
    {
        std::optional<node_t> node;
        size_t index = 0;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(invalid_position_event_t, node, index)

    // spawnedNewChildNode(Node* parent, Node* child)
    struct spawned_child_event_t
    {
        std::optional<node_t> parent;
        std::optional<node_t> child;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(spawned_child_event_t, parent, child)
}

#endif //ELOQUENTLOGIC_AGENT_CURSOR_EVENTS_HPP
