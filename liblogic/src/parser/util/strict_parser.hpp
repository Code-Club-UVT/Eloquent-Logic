//
// Created by vscode on 7/31/26.
//

#ifndef ELOQUENTLOGIC_STRICT_PARSER_HPP
#define ELOQUENTLOGIC_STRICT_PARSER_HPP

#include <vector>
#include <memory>

#include "cursor.h"
#include "lexeme.hpp"
#include "syntax_tree.hpp"
#include "parser_listener_t.hpp"
#include "lexeme_stream.hpp"
namespace eloquent::logic
{
    class strict_parser
    {
        public:
        static std::shared_ptr<syntax_tree> parse(const std::vector<lexeme>& lexemes, const std::shared_ptr<parser_listener_t>& listener);
    };
}


#endif //ELOQUENTLOGIC_STRICT_PARSER_HPP
