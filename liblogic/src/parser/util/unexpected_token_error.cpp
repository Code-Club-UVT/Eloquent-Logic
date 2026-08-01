//
// Created by vscode on 7/31/26.
//

#include "unexpected_token_error.hpp"
namespace eloquent::logic
{
    const char* unexpected_token_error::what() const noexcept
    {
        return fmt::format("Unexpected token {} at [{},{}]\n", m_lexeme.token(), m_lexeme.start(), m_lexeme.end() ).c_str();
    }
}
