//
// Created by Bogdan Petru on 05/08/2026.
//

#include "unsupported_operator_error.hpp"

#include <fmt/ostream.h>

namespace eloquent::logic {
unsupported_operator_error::unsupported_operator_error(const lexeme &l)
    : msg(fmt::format("Operator {} is not supported", l.token())) {}

const char *unsupported_operator_error::what() const noexcept {
    return msg.c_str();
}
} // namespace eloquent::logic
