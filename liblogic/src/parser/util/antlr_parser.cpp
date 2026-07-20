//
// Created by vscode on 7/20/26.
//

#include "antlr_parser.hpp"
#include <tree_sanitizer.h>
#include <LogicLex.h>
#include "syntax_tree.hpp"
namespace eloquent::logic {
    std::generator<steppable_result<parser_feedback, std::shared_ptr<syntax_tree>>> antlr_parser::step_impl() {
        antlr4::ANTLRInputStream input(this->expression);

        // Create a lexer that feeds off of input CharStream
        LogicLex lexer(&input);

        // Create a buffer of tokens pulled from the lexer
        antlr4::CommonTokenStream tokens(&lexer);

        // Create a parser that feeds off the tokens buffer
        LogicParse parser(&tokens);

        // Begin parsing at startRule
        antlr4::tree::ParseTree *tree = parser.program();

        antlr4::tree::ParseTreeWalker walker;
        // Create and use your custom visitor

        std::shared_ptr<syntax_tree> stree;
        TreeSanitizer listener(stree);
        walker.walk(&listener,tree);
        co_yield steppable_result<parser_feedback, std::shared_ptr<syntax_tree>>::with_result(stree);


    }

    antlr_parser::antlr_parser(std::string_view expression): expression(expression) {
    }

    antlr_parser antlr_parser::make(std::string_view expression) {
        return antlr_parser(expression);
    }
}
