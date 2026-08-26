//
// Created by Bogdan Petru on 27/08/2026.
//

#ifndef ELOQUENTLOGIC_INCOMPATIBLE_TOP_LEVEL_OPERATOR_HPP
#define ELOQUENTLOGIC_INCOMPATIBLE_TOP_LEVEL_OPERATOR_HPP
#include <exception>
#include <format>
#include <string>

#include "node.h"

namespace eloquent {
namespace logic {
class incompatible_top_level_operator : std::exception {
    std::string message;

  public:
    incompatible_top_level_operator(NodeObsPtr ptr)
        : message(
              fmt::format("Root node with id {} is not logical equivalence\n",
                          ptr->getUUID().string())) {}

    [[nodiscard]] const char *what() const noexcept override {
        return message.c_str();
    }
};
} // namespace logic
} // namespace eloquent

#endif // ELOQUENTLOGIC_INCOMPATIBLE_TOP_LEVEL_OPERATOR_HPP
