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
namespace eloquent::logic
{
    class strict_parser
    {
        class lexeme_stream
        {
            size_t idx = 0;
            std::vector<lexeme> lexemes;
            public:
            bool can_continue() const noexcept;
            explicit lexeme_stream(std::vector<lexeme> lexemes);
            lexeme peek();
            lexeme next();
        };
        public:
        static NodePtr parse_paranthesised_expression(Cursor& cursor, lexeme_stream& ls, const std::shared_ptr<parser_listener_t>& listener);
        static std::shared_ptr<syntax_tree> parse(const std::vector<lexeme>& lexemes, const std::shared_ptr<parser_listener_t>& listener);
    };
}


#endif //ELOQUENTLOGIC_STRICT_PARSER_HPP
