//
// Created by vscode on 7/31/26.
//

#include "unbound_variable_error.hpp"
namespace eloquent::logic {
const char *unbound_variable_error::what() const noexcept {
    return message.c_str();
}
} // namespace eloquent::logic
