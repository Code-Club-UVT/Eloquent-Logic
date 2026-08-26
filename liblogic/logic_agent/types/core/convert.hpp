#ifndef ELOQUENTLOGIC_AGENT_CONVERT_HPP
#define ELOQUENTLOGIC_AGENT_CONVERT_HPP

#include <optional>

#include <cursor.h>
#include <lexeme.hpp>
#include <node.h>
#include <system/uuid.h>

#include "../parser/cursor_t.hpp"
#include "lexeme_t.hpp"
#include "node_t.hpp"
#include "uuid_t.hpp"

// Conversion functions from the library's live/internal types
// (eloquent::logic::lexeme, Node, Cursor, CppCommon::UUID) to the plain,
// JSON-serializable DTOs in logic_agent::types. These are the only places
// in logic_agent/types that depend on liblogic's real headers.
namespace logic_agent::types {
uuid_t to_dto(const CppCommon::UUID &id);

lexeme_t to_dto(const eloquent::logic::lexeme &l);

// Recursively converts a Node and its children into an owned node_t tree.
node_t to_dto(const eloquent::logic::Node &node);

// Null-safe overload for the raw NodeObsPtr the listener callbacks pass.
std::optional<node_t> to_dto(eloquent::logic::NodeObsPtr node);

parser::cursor_t to_dto(const eloquent::logic::Cursor &cursor);
} // namespace logic_agent::types

#endif // ELOQUENTLOGIC_AGENT_CONVERT_HPP
