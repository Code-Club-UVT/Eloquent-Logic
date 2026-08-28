//
// Created by Bogdan Petru on 05/08/2026.
//

#ifndef ELOQUENTLOGIC_UNSUPPORTED_OPERATOR_ERROR_HPP
#define ELOQUENTLOGIC_UNSUPPORTED_OPERATOR_ERROR_HPP
#include <exception>
#include <string>

#include "lexeme.hpp"

namespace eloquent::logic {
class unsupported_operator_error : public std::exception {
    std::string msg;

  public:
    explicit unsupported_operator_error(const lexeme &l);
    [[nodiscard]] const char *what() const noexcept final;
};

} // namespace eloquent::logic

#endif // ELOQUENTLOGIC_UNSUPPORTED_OPERATOR_ERROR_HPP
