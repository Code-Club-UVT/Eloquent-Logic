
// Generated from LogicParse.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  LogicParse : public antlr4::Parser {
public:
  enum {
    EXP_BEGIN = 1, EXP_END = 2, NEGATION = 3, AND = 4, OR = 5, IMPLIES = 6, 
    IFF = 7, VARIABLE = 8, WS = 9
  };

  enum {
    RuleProgram = 0, RuleAtomic_expression = 1, RuleExp_begin = 2, RuleExp_end = 3, 
    RuleStrict = 4, RuleNegation_operator = 5, RuleAnd_operator = 6, RuleOr_operator = 7, 
    RuleImplies_operator = 8, RuleIff_operator = 9, RuleNegated_expression = 10, 
    RuleRelaxed = 11, RuleRelaxedInnerExpr = 12
  };

  explicit LogicParse(antlr4::TokenStream *input);

  LogicParse(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~LogicParse() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class ProgramContext;
  class Atomic_expressionContext;
  class Exp_beginContext;
  class Exp_endContext;
  class StrictContext;
  class Negation_operatorContext;
  class And_operatorContext;
  class Or_operatorContext;
  class Implies_operatorContext;
  class Iff_operatorContext;
  class Negated_expressionContext;
  class RelaxedContext;
  class RelaxedInnerExprContext; 

  class  ProgramContext : public antlr4::ParserRuleContext {
  public:
    ProgramContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    StrictContext *strict();
    RelaxedContext *relaxed();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProgramContext* program();

  class  Atomic_expressionContext : public antlr4::ParserRuleContext {
  public:
    Atomic_expressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VARIABLE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Atomic_expressionContext* atomic_expression();

  class  Exp_beginContext : public antlr4::ParserRuleContext {
  public:
    Exp_beginContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXP_BEGIN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Exp_beginContext* exp_begin();

  class  Exp_endContext : public antlr4::ParserRuleContext {
  public:
    Exp_endContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXP_END();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Exp_endContext* exp_end();

  class  StrictContext : public antlr4::ParserRuleContext {
  public:
    StrictContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Atomic_expressionContext *atomic_expression();
    Exp_beginContext *exp_begin();
    Negation_operatorContext *negation_operator();
    std::vector<StrictContext *> strict();
    StrictContext* strict(size_t i);
    Exp_endContext *exp_end();
    And_operatorContext *and_operator();
    Or_operatorContext *or_operator();
    Implies_operatorContext *implies_operator();
    Iff_operatorContext *iff_operator();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StrictContext* strict();

  class  Negation_operatorContext : public antlr4::ParserRuleContext {
  public:
    Negation_operatorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NEGATION();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Negation_operatorContext* negation_operator();

  class  And_operatorContext : public antlr4::ParserRuleContext {
  public:
    And_operatorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AND();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  And_operatorContext* and_operator();

  class  Or_operatorContext : public antlr4::ParserRuleContext {
  public:
    Or_operatorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OR();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Or_operatorContext* or_operator();

  class  Implies_operatorContext : public antlr4::ParserRuleContext {
  public:
    Implies_operatorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IMPLIES();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Implies_operatorContext* implies_operator();

  class  Iff_operatorContext : public antlr4::ParserRuleContext {
  public:
    Iff_operatorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IFF();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Iff_operatorContext* iff_operator();

  class  Negated_expressionContext : public antlr4::ParserRuleContext {
  public:
    Negated_expressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Atomic_expressionContext *atomic_expression();
    Exp_beginContext *exp_begin();
    RelaxedInnerExprContext *relaxedInnerExpr();
    Exp_endContext *exp_end();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Negated_expressionContext* negated_expression();

  class  RelaxedContext : public antlr4::ParserRuleContext {
  public:
    RelaxedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Negation_operatorContext *negation_operator();
    Negated_expressionContext *negated_expression();
    Exp_beginContext *exp_begin();
    RelaxedInnerExprContext *relaxedInnerExpr();
    Exp_endContext *exp_end();
    Atomic_expressionContext *atomic_expression();
    std::vector<RelaxedContext *> relaxed();
    RelaxedContext* relaxed(size_t i);
    And_operatorContext *and_operator();
    Or_operatorContext *or_operator();
    Implies_operatorContext *implies_operator();
    Iff_operatorContext *iff_operator();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RelaxedContext* relaxed();
  RelaxedContext* relaxed(int precedence);
  class  RelaxedInnerExprContext : public antlr4::ParserRuleContext {
  public:
    RelaxedInnerExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    RelaxedInnerExprContext() = default;
    void copyFrom(RelaxedInnerExprContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  FurtherNestedExprContext : public RelaxedInnerExprContext {
  public:
    FurtherNestedExprContext(RelaxedInnerExprContext *ctx);

    Exp_beginContext *exp_begin();
    RelaxedInnerExprContext *relaxedInnerExpr();
    Exp_endContext *exp_end();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  NestedAndExprContext : public RelaxedInnerExprContext {
  public:
    NestedAndExprContext(RelaxedInnerExprContext *ctx);

    std::vector<RelaxedContext *> relaxed();
    RelaxedContext* relaxed(size_t i);
    And_operatorContext *and_operator();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  NestedOrExprContext : public RelaxedInnerExprContext {
  public:
    NestedOrExprContext(RelaxedInnerExprContext *ctx);

    std::vector<RelaxedContext *> relaxed();
    RelaxedContext* relaxed(size_t i);
    Or_operatorContext *or_operator();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  NestedImpliesExprContext : public RelaxedInnerExprContext {
  public:
    NestedImpliesExprContext(RelaxedInnerExprContext *ctx);

    std::vector<RelaxedContext *> relaxed();
    RelaxedContext* relaxed(size_t i);
    Implies_operatorContext *implies_operator();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  NestedIffExprContext : public RelaxedInnerExprContext {
  public:
    NestedIffExprContext(RelaxedInnerExprContext *ctx);

    std::vector<RelaxedContext *> relaxed();
    RelaxedContext* relaxed(size_t i);
    Iff_operatorContext *iff_operator();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  NestedNotExprContext : public RelaxedInnerExprContext {
  public:
    NestedNotExprContext(RelaxedInnerExprContext *ctx);

    Negation_operatorContext *negation_operator();
    RelaxedContext *relaxed();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  RelaxedInnerExprContext* relaxedInnerExpr();


  bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;

  bool relaxedSempred(RelaxedContext *_localctx, size_t predicateIndex);

  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

