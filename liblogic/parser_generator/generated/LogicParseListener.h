
// Generated from LogicParse.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "LogicParse.h"


/**
 * This interface defines an abstract listener for a parse tree produced by LogicParse.
 */
class  LogicParseListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterProgram(LogicParse::ProgramContext *ctx) = 0;
  virtual void exitProgram(LogicParse::ProgramContext *ctx) = 0;

  virtual void enterAtomic_expression(LogicParse::Atomic_expressionContext *ctx) = 0;
  virtual void exitAtomic_expression(LogicParse::Atomic_expressionContext *ctx) = 0;

  virtual void enterExp_begin(LogicParse::Exp_beginContext *ctx) = 0;
  virtual void exitExp_begin(LogicParse::Exp_beginContext *ctx) = 0;

  virtual void enterExp_end(LogicParse::Exp_endContext *ctx) = 0;
  virtual void exitExp_end(LogicParse::Exp_endContext *ctx) = 0;

  virtual void enterStrict(LogicParse::StrictContext *ctx) = 0;
  virtual void exitStrict(LogicParse::StrictContext *ctx) = 0;

  virtual void enterNegation_operator(LogicParse::Negation_operatorContext *ctx) = 0;
  virtual void exitNegation_operator(LogicParse::Negation_operatorContext *ctx) = 0;

  virtual void enterAnd_operator(LogicParse::And_operatorContext *ctx) = 0;
  virtual void exitAnd_operator(LogicParse::And_operatorContext *ctx) = 0;

  virtual void enterOr_operator(LogicParse::Or_operatorContext *ctx) = 0;
  virtual void exitOr_operator(LogicParse::Or_operatorContext *ctx) = 0;

  virtual void enterImplies_operator(LogicParse::Implies_operatorContext *ctx) = 0;
  virtual void exitImplies_operator(LogicParse::Implies_operatorContext *ctx) = 0;

  virtual void enterIff_operator(LogicParse::Iff_operatorContext *ctx) = 0;
  virtual void exitIff_operator(LogicParse::Iff_operatorContext *ctx) = 0;

  virtual void enterNegated_expression(LogicParse::Negated_expressionContext *ctx) = 0;
  virtual void exitNegated_expression(LogicParse::Negated_expressionContext *ctx) = 0;

  virtual void enterRelaxed(LogicParse::RelaxedContext *ctx) = 0;
  virtual void exitRelaxed(LogicParse::RelaxedContext *ctx) = 0;

  virtual void enterNestedAndExpr(LogicParse::NestedAndExprContext *ctx) = 0;
  virtual void exitNestedAndExpr(LogicParse::NestedAndExprContext *ctx) = 0;

  virtual void enterNestedOrExpr(LogicParse::NestedOrExprContext *ctx) = 0;
  virtual void exitNestedOrExpr(LogicParse::NestedOrExprContext *ctx) = 0;

  virtual void enterNestedImpliesExpr(LogicParse::NestedImpliesExprContext *ctx) = 0;
  virtual void exitNestedImpliesExpr(LogicParse::NestedImpliesExprContext *ctx) = 0;

  virtual void enterNestedIffExpr(LogicParse::NestedIffExprContext *ctx) = 0;
  virtual void exitNestedIffExpr(LogicParse::NestedIffExprContext *ctx) = 0;

  virtual void enterNestedNotExpr(LogicParse::NestedNotExprContext *ctx) = 0;
  virtual void exitNestedNotExpr(LogicParse::NestedNotExprContext *ctx) = 0;

  virtual void enterFurtherNestedExpr(LogicParse::FurtherNestedExprContext *ctx) = 0;
  virtual void exitFurtherNestedExpr(LogicParse::FurtherNestedExprContext *ctx) = 0;


};

