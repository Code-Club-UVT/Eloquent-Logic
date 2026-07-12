
// Generated from LogicParse.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "LogicParse.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by LogicParse.
 */
class  LogicParseVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by LogicParse.
   */
    virtual std::any visitProgram(LogicParse::ProgramContext *context) = 0;

    virtual std::any visitAtomic_expression(LogicParse::Atomic_expressionContext *context) = 0;

    virtual std::any visitExp_begin(LogicParse::Exp_beginContext *context) = 0;

    virtual std::any visitExp_end(LogicParse::Exp_endContext *context) = 0;

    virtual std::any visitStrict(LogicParse::StrictContext *context) = 0;

    virtual std::any visitNegation_operator(LogicParse::Negation_operatorContext *context) = 0;

    virtual std::any visitAnd_operator(LogicParse::And_operatorContext *context) = 0;

    virtual std::any visitOr_operator(LogicParse::Or_operatorContext *context) = 0;

    virtual std::any visitImplies_operator(LogicParse::Implies_operatorContext *context) = 0;

    virtual std::any visitIff_operator(LogicParse::Iff_operatorContext *context) = 0;

    virtual std::any visitNegated_expression(LogicParse::Negated_expressionContext *context) = 0;

    virtual std::any visitRelaxed(LogicParse::RelaxedContext *context) = 0;

    virtual std::any visitNestedAndExpr(LogicParse::NestedAndExprContext *context) = 0;

    virtual std::any visitNestedOrExpr(LogicParse::NestedOrExprContext *context) = 0;

    virtual std::any visitNestedImpliesExpr(LogicParse::NestedImpliesExprContext *context) = 0;

    virtual std::any visitNestedIffExpr(LogicParse::NestedIffExprContext *context) = 0;

    virtual std::any visitNestedNotExpr(LogicParse::NestedNotExprContext *context) = 0;

    virtual std::any visitFurtherNestedExpr(LogicParse::FurtherNestedExprContext *context) = 0;


};

