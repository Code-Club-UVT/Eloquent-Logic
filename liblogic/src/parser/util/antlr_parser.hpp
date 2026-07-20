//
// Created by vscode on 7/20/26.
//

#ifndef ELOQUENTLOGIC_ANTLR_PARSER_HPP
#define ELOQUENTLOGIC_ANTLR_PARSER_HPP
#include <steppable.hpp>
#include <steppable_result.hpp>
#include <atn/ATN.h>

#include "syntax_tree.hpp"
namespace eloquent::logic {

    class parser_feedback : public feedback_base {

    };
    class antlr_parser : public steppable<parser_feedback, std::shared_ptr<syntax_tree>> {
        std::string_view expression;

    protected:
        std::generator<steppable_result<parser_feedback, std::shared_ptr<syntax_tree>>> step_impl() final;

        explicit antlr_parser(std::string_view expression);
    public:
        static antlr_parser make(std::string_view expression);

        antlr_parser() = default;

    };
}

#endif //ELOQUENTLOGIC_ANTLR_PARSER_HPP
