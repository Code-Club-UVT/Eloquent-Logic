//
// Created by vscode on 7/31/26.
//

#ifndef ELOQUENTLOGIC_STRICT_PARSER_HPP
#define ELOQUENTLOGIC_STRICT_PARSER_HPP

#include <vector>

#include "lexeme.hpp"
#include "syntax_tree.hpp"
namespace eloquent::logic
{
    class strict_parser
    {
        public:
        static syntax_tree parse(const std::vector<lexeme>& lexemes);
    };
}


#endif //ELOQUENTLOGIC_STRICT_PARSER_HPP
