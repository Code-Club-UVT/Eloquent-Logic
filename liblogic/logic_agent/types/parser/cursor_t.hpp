#ifndef ELOQUENTLOGIC_AGENT_CURSOR_T_HPP
#define ELOQUENTLOGIC_AGENT_CURSOR_T_HPP

#include <optional>

#include <nlohmann/json.hpp>

#include "../core/node_t.hpp"

namespace logic_agent::types::parser {
// Mirrors eloquent::logic::Cursor (src/parser/cursor.h): a snapshot of
// where the cursor currently sits plus the root of the tree it is
// building, rather than a 1:1 mirror of the live pointer/shared_ptr
// members (which aren't meaningful outside the parsing process).
struct cursor_t {
    std::optional<node_t> current_node;
    std::optional<node_t> tree_root;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(cursor_t, current_node, tree_root)
} // namespace logic_agent::types::parser

#endif // ELOQUENTLOGIC_AGENT_CURSOR_T_HPP
