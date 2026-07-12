#pragma once



#include <iostream>
#include <LogicParseBaseListener.h>
#include <LogicParseBaseVisitor.h>
#include <liblogic/private/cursor.h>
namespace eloquent::logic{
    class TreeSanitizer : public LogicParseBaseListener{

    public:
        CursorPrivate cursor;
        void enterEveryRule(antlr4::ParserRuleContext*) override;

        void enterAtomic_expression(LogicParse::Atomic_expressionContext *) override;

        void enterNegation_operator(LogicParse::Negation_operatorContext *) override;

        void enterAnd_operator(LogicParse::And_operatorContext *) override;

        void enterOr_operator(LogicParse::Or_operatorContext *) override;

        void enterImplies_operator(LogicParse::Implies_operatorContext *) override;

        void enterIff_operator(LogicParse::Iff_operatorContext *) override;

        void enterStrict(LogicParse::StrictContext *) override;

        void enterRelaxed(LogicParse::RelaxedContext *) override;
    };


};

