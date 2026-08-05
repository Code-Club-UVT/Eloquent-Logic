//
// Created by vscode on 7/31/26.
//

#include "unexpected_token_error.hpp"
namespace eloquent::logic
{
    const char* unexpected_token_error::what() const noexcept
    {
        return message.c_str();
    }
}
