#ifndef ELOQUENTLOGIC_AGENT_UUID_T_HPP
#define ELOQUENTLOGIC_AGENT_UUID_T_HPP

#include <string>

#include <nlohmann/json.hpp>

namespace logic_agent::types
{
    // JSON-serializable mirror of CppCommon::UUID (src/parser/node.h),
    // stored in its canonical "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" form.
    struct uuid_t
    {
        std::string value;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(uuid_t, value)
}

#endif //ELOQUENTLOGIC_AGENT_UUID_T_HPP
