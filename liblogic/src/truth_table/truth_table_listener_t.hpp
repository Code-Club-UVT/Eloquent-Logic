//
// Created by Bogdan Petru on 05/08/2026.
//

#ifndef ELOQUENTLOGIC_TRUTH_TABLE_LISTENER_T_HPP
#define ELOQUENTLOGIC_TRUTH_TABLE_LISTENER_T_HPP
#include <errors/exceptions.h>
#include <system/uuid.h>

#include "feedback_listener.hpp"
#include "lexeme.hpp"

namespace eloquent::logic {
class truth_table_listener_t : public feedback_listener {
  public:
    virtual void didFindUnknownVariable(const std::string_view &) {}
    virtual void didSetVariable(const std::string_view &, bool) {}
    virtual void didComputeSubexpression(const CppCommon::UUID &, bool) {}
};
} // namespace eloquent::logic

#endif // ELOQUENTLOGIC_TRUTH_TABLE_LISTENER_T_HPP
