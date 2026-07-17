
// Generated from LogicParse.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "LogicParseListener.h"


/**
 * This class provides an empty implementation of LogicParseListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  LogicParseBaseListener : public LogicParseListener {
public:

  virtual void enterProgram(LogicParse::ProgramContext * /*ctx*/) override { }
  virtual void exitProgram(LogicParse::ProgramContext * /*ctx*/) override { }

  virtual void enterAtomic_expression(LogicParse::Atomic_expressionContext * /*ctx*/) override { }
  virtual void exitAtomic_expression(LogicParse::Atomic_expressionContext * /*ctx*/) override { }

  virtual void enterExp_begin(LogicParse::Exp_beginContext * /*ctx*/) override { }
  virtual void exitExp_begin(LogicParse::Exp_beginContext * /*ctx*/) override { }

  virtual void enterExp_end(LogicParse::Exp_endContext * /*ctx*/) override { }
  virtual void exitExp_end(LogicParse::Exp_endContext * /*ctx*/) override { }

  virtual void enterStrict(LogicParse::StrictContext * /*ctx*/) override { }
  virtual void exitStrict(LogicParse::StrictContext * /*ctx*/) override { }

  virtual void enterNegation_operator(LogicParse::Negation_operatorContext * /*ctx*/) override { }
  virtual void exitNegation_operator(LogicParse::Negation_operatorContext * /*ctx*/) override { }

  virtual void enterAnd_operator(LogicParse::And_operatorContext * /*ctx*/) override { }
  virtual void exitAnd_operator(LogicParse::And_operatorContext * /*ctx*/) override { }

  virtual void enterOr_operator(LogicParse::Or_operatorContext * /*ctx*/) override { }
  virtual void exitOr_operator(LogicParse::Or_operatorContext * /*ctx*/) override { }

  virtual void enterImplies_operator(LogicParse::Implies_operatorContext * /*ctx*/) override { }
  virtual void exitImplies_operator(LogicParse::Implies_operatorContext * /*ctx*/) override { }

  virtual void enterIff_operator(LogicParse::Iff_operatorContext * /*ctx*/) override { }
  virtual void exitIff_operator(LogicParse::Iff_operatorContext * /*ctx*/) override { }

  virtual void enterNegated_expression(LogicParse::Negated_expressionContext * /*ctx*/) override { }
  virtual void exitNegated_expression(LogicParse::Negated_expressionContext * /*ctx*/) override { }

  virtual void enterRelaxed(LogicParse::RelaxedContext * /*ctx*/) override { }
  virtual void exitRelaxed(LogicParse::RelaxedContext * /*ctx*/) override { }

  virtual void enterNestedAndExpr(LogicParse::NestedAndExprContext * /*ctx*/) override { }
  virtual void exitNestedAndExpr(LogicParse::NestedAndExprContext * /*ctx*/) override { }

  virtual void enterNestedOrExpr(LogicParse::NestedOrExprContext * /*ctx*/) override { }
  virtual void exitNestedOrExpr(LogicParse::NestedOrExprContext * /*ctx*/) override { }

  virtual void enterNestedImpliesExpr(LogicParse::NestedImpliesExprContext * /*ctx*/) override { }
  virtual void exitNestedImpliesExpr(LogicParse::NestedImpliesExprContext * /*ctx*/) override { }

  virtual void enterNestedIffExpr(LogicParse::NestedIffExprContext * /*ctx*/) override { }
  virtual void exitNestedIffExpr(LogicParse::NestedIffExprContext * /*ctx*/) override { }

  virtual void enterNestedNotExpr(LogicParse::NestedNotExprContext * /*ctx*/) override { }
  virtual void exitNestedNotExpr(LogicParse::NestedNotExprContext * /*ctx*/) override { }

  virtual void enterFurtherNestedExpr(LogicParse::FurtherNestedExprContext * /*ctx*/) override { }
  virtual void exitFurtherNestedExpr(LogicParse::FurtherNestedExprContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

