#ifndef ELOQUENTLOGIC_AGENT_SAT_COMMON_T_HPP
#define ELOQUENTLOGIC_AGENT_SAT_COMMON_T_HPP

#include <cstdint>
#include <vector>

// JSON-serializable mirrors of the SAT type aliases in
// src/sat/utilities/common.h. Literal is already a JSON-friendly scalar;
// Clause/ClauseSet (std::set<Literal>/std::set<Clause>) are mirrored as
// vectors so they serialize as plain JSON arrays. All three are plain
// container/alias types nlohmann::json already knows how to (de)serialize,
// so no NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE macro is needed here.
namespace logic_agent::types::sat
{
    using literal_t = std::int64_t;
    using clause_t = std::vector<literal_t>;
    using clause_set_t = std::vector<clause_t>;
}

#endif //ELOQUENTLOGIC_AGENT_SAT_COMMON_T_HPP
