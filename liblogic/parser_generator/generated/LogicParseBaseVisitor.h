
// Generated from LogicParse.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "LogicParseVisitor.h"


/**
 * This class provides an empty implementation of LogicParseVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  LogicParseBaseVisitor : public LogicParseVisitor {
public:

  virtual std::any visitProgram(LogicParse::ProgramContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAtomic_expression(LogicParse::Atomic_expressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExp_begin(LogicParse::Exp_beginContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExp_end(LogicParse::Exp_endContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStrict(LogicParse::StrictContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNegation_operator(LogicParse::Negation_operatorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAnd_operator(LogicParse::And_operatorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOr_operator(LogicParse::Or_operatorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitImplies_operator(LogicParse::Implies_operatorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIff_operator(LogicParse::Iff_operatorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNegated_expression(LogicParse::Negated_expressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelaxed(LogicParse::RelaxedContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNestedAndExpr(LogicParse::NestedAndExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNestedOrExpr(LogicParse::NestedOrExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNestedImpliesExpr(LogicParse::NestedImpliesExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNestedIffExpr(LogicParse::NestedIffExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNestedNotExpr(LogicParse::NestedNotExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFurtherNestedExpr(LogicParse::FurtherNestedExprContext *ctx) override {
    return visitChildren(ctx);
  }


};

