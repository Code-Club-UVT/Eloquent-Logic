#ifndef ELOQUENTLOGIC_AGENT_UUID_T_HPP
#define ELOQUENTLOGIC_AGENT_UUID_T_HPP

#include <string>

#include <nlohmann/json.hpp>

namespace logic_agent::types {
// JSON-serializable mirror of CppCommon::UUID (src/parser/node.h),
// stored in its canonical "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" form.
struct uuid_t {
    std::string value;
};

// Hand-written rather than NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE: uuid_t
// serializes as a bare JSON string (e.g. "id": "xxxxxxxx-...") rather
// than as a {"value": "..."} subobject, so every field/element of this
// type — node_t::id, node_t::children, subexpression_result_event_t::node_id —
// picks up the same flat representation via nlohmann's ADL lookup.
inline void to_json(nlohmann::json &j, const uuid_t &u) { j = u.value; }

inline void from_json(const nlohmann::json &j, uuid_t &u) {
    u.value = j.get<std::string>();
}
} // namespace logic_agent::types

#endif // ELOQUENTLOGIC_AGENT_UUID_T_HPP
