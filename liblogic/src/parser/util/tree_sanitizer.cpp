//
// Created by Bogdan Petru on 05.12.2024.
//

#include "tree_sanitizer.h"
#include <LogicParse.h>
#include <iostream>
#include <utility>


namespace eloquent::logic {
    void TreeSanitizer::enterEveryRule(antlr4::ParserRuleContext* ctx)
    {
        std::cout<<"Rule: ";
        switch (ctx->getRuleIndex())
        {
            case LogicParse::RuleProgram:
                std::cout <<"Program: "<<ctx->getText()<<'\n';
            break;
            case LogicParse::RuleRelaxed:
                std::cout<<"Relaxed: "<<ctx->getText()<<'\n';
            break;
            case LogicParse::RuleStrict:
                std::cout<<"Strict: "<<ctx->getText()<<'\n';
            break;
            case LogicParse::RuleRelaxedInnerExpr:
                std::cout<<"InnerExpr: "<<ctx->getText()<<'\n';
            break;
            case LogicParse::RuleAnd_operator:
                std::cout<<"AndOp: "<<ctx->getText()<<'\n';
            break;
            case LogicParse::RuleAtomic_expression:
                std::cout<<"Atom: "<<ctx->getText()<<'\n';
            break;
            case LogicParse::RuleNegation_operator:
                std::cout<<"NotOp: "<<ctx->getText()<<'\n';
            break;
            case LogicParse::RuleOr_operator:
                std::cout<<"OrOp: "<<ctx->getText()<<'\n';
            break;
            case LogicParse::RuleImplies_operator:
                std::cout<<"ImpliesOp: "<<ctx->getText()<<'\n';
            break;
            case LogicParse::RuleIff_operator:
                std::cout<<"IffOp: "<<ctx->getText()<<'\n';
            break;
            case LogicParse::RuleExp_begin:
                std::cout<<"ExpBeginOp: "<< ctx->getText()<<'\n';
            break;
            case LogicParse::RuleExp_end:
                std::cout<<"ExpEndOp: "<<ctx->getText()<<'\n';
            break;
        }
    }


    void TreeSanitizer::enterAtomic_expression(
        LogicParse::Atomic_expressionContext *atomic_expression_context) {
        cursor.set_node(NodeBuilder::makeNewAtomicNode(atomic_expression_context->getText()));
        cursor.up();
    }

    void TreeSanitizer::enterNegation_operator(
        LogicParse::Negation_operatorContext *negation_operator_context) {
        cursor.set_node(NodeBuilder::makeNewNotNode());
    }

    void TreeSanitizer::enterAnd_operator(LogicParse::And_operatorContext *and_operator_context) {
        cursor.set_node(NodeBuilder::makeNewAndNode());
    }

    void TreeSanitizer::enterOr_operator(LogicParse::Or_operatorContext *or_operator_context) {
        cursor.set_node(NodeBuilder::makeNewOrNode());
    }

    void TreeSanitizer::enterImplies_operator(
        LogicParse::Implies_operatorContext *implies_operator_context) {
        cursor.set_node(NodeBuilder::makeNewImpliesNode());
    }

    void TreeSanitizer::enterIff_operator(LogicParse::Iff_operatorContext *iff_operator_context) {
        cursor.set_node(NodeBuilder::makeNewIffNode());
    }
    void TreeSanitizer::enterStrict(LogicParse::StrictContext *strict_context) {
        cursor.spawn_new_child_node();
    }

    void TreeSanitizer::enterRelaxed(LogicParse::RelaxedContext *relaxed_context) {
        cursor.spawn_new_child_node();
    }

    TreeSanitizer::TreeSanitizer(std::shared_ptr<syntax_tree> tree): cursor(std::move(tree)){
    }
};