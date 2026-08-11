#ifndef ELOQUENTLOGIC_AGENT_NODE_T_HPP
#define ELOQUENTLOGIC_AGENT_NODE_T_HPP

#include <vector>

#include <nlohmann/json.hpp>

#include "lexeme_t.hpp"
#include "uuid_t.hpp"

namespace logic_agent::types
{
    // Mirrors eloquent::logic::NodeType (src/parser/node.h).
    // `Blank` is listed first so it is the fallback value nlohmann's
    // enum macro uses when deserializing an unrecognised string.
    enum class node_type_t
    {
        Blank,
        Atom,
        NotOp,
        AndOp,
        OrOp,
        ImpliesOp,
        IffOp,
        LEquiOp
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(node_type_t, {
        {node_type_t::Blank, "Blank"},
        {node_type_t::Atom, "Atom"},
        {node_type_t::NotOp, "NotOp"},
        {node_type_t::AndOp, "AndOp"},
        {node_type_t::OrOp, "OrOp"},
        {node_type_t::ImpliesOp, "ImpliesOp"},
        {node_type_t::IffOp, "IffOp"},
        {node_type_t::LEquiOp, "LEquiOp"},
    })

    // Mirrors eloquent::logic::Node (src/parser/node.h). Unlike the
    // library's Node, this is a plain value type: no parent pointer (would
    // be a cycle in JSON), and children are embedded by value rather than
    // referenced by pointer/uuid, since expression ASTs in this domain are
    // small.
    struct node_t
    {
        uuid_t id;
        node_type_t type = node_type_t::Blank;
        lexeme_t lexeme;
        std::vector<node_t> children;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(node_t, id, type, lexeme, children)
}

#endif //ELOQUENTLOGIC_AGENT_NODE_T_HPP
