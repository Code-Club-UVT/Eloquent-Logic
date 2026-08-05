//
// Created by vscode on 7/31/26.
//

#include "unknown_variable_error.hpp"
namespace eloquent::logic
{
    const char* unknown_variable_error::what() const noexcept
    {
        return message.c_str();
    }
}
