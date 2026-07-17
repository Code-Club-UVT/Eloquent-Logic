
// Generated from LogicParse.g4 by ANTLR 4.13.2


#include "LogicParseListener.h"
#include "LogicParseVisitor.h"

#include "LogicParse.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct LogicParseStaticData final {
  LogicParseStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  LogicParseStaticData(const LogicParseStaticData&) = delete;
  LogicParseStaticData(LogicParseStaticData&&) = delete;
  LogicParseStaticData& operator=(const LogicParseStaticData&) = delete;
  LogicParseStaticData& operator=(LogicParseStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag logicparseParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<LogicParseStaticData> logicparseParserStaticData = nullptr;

void logicparseParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (logicparseParserStaticData != nullptr) {
    return;
  }
#else
  assert(logicparseParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<LogicParseStaticData>(
    std::vector<std::string>{
      "program", "atomic_expression", "exp_begin", "exp_end", "strict", 
      "negation_operator", "and_operator", "or_operator", "implies_operator", 
      "iff_operator", "negated_expression", "relaxed", "relaxedInnerExpr"
    },
    std::vector<std::string>{
      "", "'('", "')'", "'\\\\neg'", "'\\\\wedge'", "'\\\\vee'", "'\\\\implies'", 
      "'\\\\iff'"
    },
    std::vector<std::string>{
      "", "EXP_BEGIN", "EXP_END", "NEGATION", "AND", "OR", "IMPLIES", "IFF", 
      "VARIABLE", "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,9,145,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,7,
  	7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,1,0,1,0,3,0,29,8,0,
  	1,0,1,0,1,1,1,1,1,2,1,2,1,3,1,3,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,
  	4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,
  	1,4,1,4,1,4,3,4,69,8,4,1,5,1,5,1,6,1,6,1,7,1,7,1,8,1,8,1,9,1,9,1,10,1,
  	10,1,10,1,10,1,10,3,10,86,8,10,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,
  	1,11,3,11,97,8,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,
  	11,1,11,1,11,1,11,1,11,1,11,5,11,115,8,11,10,11,12,11,118,9,11,1,12,1,
  	12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,
  	12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,3,12,143,8,12,1,12,0,1,22,13,0,
  	2,4,6,8,10,12,14,16,18,20,22,24,0,0,149,0,28,1,0,0,0,2,32,1,0,0,0,4,34,
  	1,0,0,0,6,36,1,0,0,0,8,68,1,0,0,0,10,70,1,0,0,0,12,72,1,0,0,0,14,74,1,
  	0,0,0,16,76,1,0,0,0,18,78,1,0,0,0,20,85,1,0,0,0,22,96,1,0,0,0,24,142,
  	1,0,0,0,26,29,3,8,4,0,27,29,3,22,11,0,28,26,1,0,0,0,28,27,1,0,0,0,29,
  	30,1,0,0,0,30,31,5,0,0,1,31,1,1,0,0,0,32,33,5,8,0,0,33,3,1,0,0,0,34,35,
  	5,1,0,0,35,5,1,0,0,0,36,37,5,2,0,0,37,7,1,0,0,0,38,69,3,2,1,0,39,40,3,
  	4,2,0,40,41,3,10,5,0,41,42,3,8,4,0,42,43,3,6,3,0,43,69,1,0,0,0,44,45,
  	3,4,2,0,45,46,3,8,4,0,46,47,3,12,6,0,47,48,3,8,4,0,48,49,3,6,3,0,49,69,
  	1,0,0,0,50,51,3,4,2,0,51,52,3,8,4,0,52,53,3,14,7,0,53,54,3,8,4,0,54,55,
  	3,6,3,0,55,69,1,0,0,0,56,57,3,4,2,0,57,58,3,8,4,0,58,59,3,16,8,0,59,60,
  	3,8,4,0,60,61,3,6,3,0,61,69,1,0,0,0,62,63,3,4,2,0,63,64,3,8,4,0,64,65,
  	3,18,9,0,65,66,3,8,4,0,66,67,3,6,3,0,67,69,1,0,0,0,68,38,1,0,0,0,68,39,
  	1,0,0,0,68,44,1,0,0,0,68,50,1,0,0,0,68,56,1,0,0,0,68,62,1,0,0,0,69,9,
  	1,0,0,0,70,71,5,3,0,0,71,11,1,0,0,0,72,73,5,4,0,0,73,13,1,0,0,0,74,75,
  	5,5,0,0,75,15,1,0,0,0,76,77,5,6,0,0,77,17,1,0,0,0,78,79,5,7,0,0,79,19,
  	1,0,0,0,80,86,3,2,1,0,81,82,3,4,2,0,82,83,3,24,12,0,83,84,3,6,3,0,84,
  	86,1,0,0,0,85,80,1,0,0,0,85,81,1,0,0,0,86,21,1,0,0,0,87,88,6,11,-1,0,
  	88,89,3,10,5,0,89,90,3,20,10,0,90,97,1,0,0,0,91,92,3,4,2,0,92,93,3,24,
  	12,0,93,94,3,6,3,0,94,97,1,0,0,0,95,97,3,2,1,0,96,87,1,0,0,0,96,91,1,
  	0,0,0,96,95,1,0,0,0,97,116,1,0,0,0,98,99,10,6,0,0,99,100,3,12,6,0,100,
  	101,3,22,11,6,101,115,1,0,0,0,102,103,10,5,0,0,103,104,3,14,7,0,104,105,
  	3,22,11,5,105,115,1,0,0,0,106,107,10,4,0,0,107,108,3,16,8,0,108,109,3,
  	22,11,4,109,115,1,0,0,0,110,111,10,3,0,0,111,112,3,18,9,0,112,113,3,22,
  	11,3,113,115,1,0,0,0,114,98,1,0,0,0,114,102,1,0,0,0,114,106,1,0,0,0,114,
  	110,1,0,0,0,115,118,1,0,0,0,116,114,1,0,0,0,116,117,1,0,0,0,117,23,1,
  	0,0,0,118,116,1,0,0,0,119,120,3,22,11,0,120,121,3,12,6,0,121,122,3,22,
  	11,0,122,143,1,0,0,0,123,124,3,22,11,0,124,125,3,14,7,0,125,126,3,22,
  	11,0,126,143,1,0,0,0,127,128,3,22,11,0,128,129,3,16,8,0,129,130,3,22,
  	11,0,130,143,1,0,0,0,131,132,3,22,11,0,132,133,3,18,9,0,133,134,3,22,
  	11,0,134,143,1,0,0,0,135,136,3,10,5,0,136,137,3,22,11,0,137,143,1,0,0,
  	0,138,139,3,4,2,0,139,140,3,24,12,0,140,141,3,6,3,0,141,143,1,0,0,0,142,
  	119,1,0,0,0,142,123,1,0,0,0,142,127,1,0,0,0,142,131,1,0,0,0,142,135,1,
  	0,0,0,142,138,1,0,0,0,143,25,1,0,0,0,7,28,68,85,96,114,116,142
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  logicparseParserStaticData = std::move(staticData);
}

}

LogicParse::LogicParse(TokenStream *input) : LogicParse(input, antlr4::atn::ParserATNSimulatorOptions()) {}

LogicParse::LogicParse(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  LogicParse::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *logicparseParserStaticData->atn, logicparseParserStaticData->decisionToDFA, logicparseParserStaticData->sharedContextCache, options);
}

LogicParse::~LogicParse() {
  delete _interpreter;
}

const atn::ATN& LogicParse::getATN() const {
  return *logicparseParserStaticData->atn;
}

std::string LogicParse::getGrammarFileName() const {
  return "LogicParse.g4";
}

const std::vector<std::string>& LogicParse::getRuleNames() const {
  return logicparseParserStaticData->ruleNames;
}

const dfa::Vocabulary& LogicParse::getVocabulary() const {
  return logicparseParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView LogicParse::getSerializedATN() const {
  return logicparseParserStaticData->serializedATN;
}


//----------------- ProgramContext ------------------------------------------------------------------

LogicParse::ProgramContext::ProgramContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogicParse::ProgramContext::EOF() {
  return getToken(LogicParse::EOF, 0);
}

LogicParse::StrictContext* LogicParse::ProgramContext::strict() {
  return getRuleContext<LogicParse::StrictContext>(0);
}

LogicParse::RelaxedContext* LogicParse::ProgramContext::relaxed() {
  return getRuleContext<LogicParse::RelaxedContext>(0);
}


size_t LogicParse::ProgramContext::getRuleIndex() const {
  return LogicParse::RuleProgram;
}

void LogicParse::ProgramContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterProgram(this);
}

void LogicParse::ProgramContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitProgram(this);
}


std::any LogicParse::ProgramContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicParseVisitor*>(visitor))
    return parserVisitor->visitProgram(this);
  else
    return visitor->visitChildren(this);
}

LogicParse::ProgramContext* LogicParse::program() {
  ProgramContext *_localctx = _tracker.createInstance<ProgramContext>(_ctx, getState());
  enterRule(_localctx, 0, LogicParse::RuleProgram);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(28);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx)) {
    case 1: {
      setState(26);
      strict();
      break;
    }

    case 2: {
      setState(27);
      relaxed(0);
      break;
    }

    default:
      break;
    }
    setState(30);
    match(LogicParse::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Atomic_expressionContext ------------------------------------------------------------------

LogicParse::Atomic_expressionContext::Atomic_expressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogicParse::Atomic_expressionContext::VARIABLE() {
  return getToken(LogicParse::VARIABLE, 0);
}


size_t LogicParse::Atomic_expressionContext::getRuleIndex() const {
  return LogicParse::RuleAtomic_expression;
}

void LogicParse::Atomic_expressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAtomic_expression(this);
}

void LogicParse::Atomic_expressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAtomic_expression(this);
}


std::any LogicParse::Atomic_expressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicParseVisitor*>(visitor))
    return parserVisitor->visitAtomic_expression(this);
  else
    return visitor->visitChildren(this);
}

LogicParse::Atomic_expressionContext* LogicParse::atomic_expression() {
  Atomic_expressionContext *_localctx = _tracker.createInstance<Atomic_expressionContext>(_ctx, getState());
  enterRule(_localctx, 2, LogicParse::RuleAtomic_expression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(32);
    match(LogicParse::VARIABLE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Exp_beginContext ------------------------------------------------------------------

LogicParse::Exp_beginContext::Exp_beginContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogicParse::Exp_beginContext::EXP_BEGIN() {
  return getToken(LogicParse::EXP_BEGIN, 0);
}


size_t LogicParse::Exp_beginContext::getRuleIndex() const {
  return LogicParse::RuleExp_begin;
}

void LogicParse::Exp_beginContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExp_begin(this);
}

void LogicParse::Exp_beginContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExp_begin(this);
}


std::any LogicParse::Exp_beginContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicParseVisitor*>(visitor))
    return parserVisitor->visitExp_begin(this);
  else
    return visitor->visitChildren(this);
}

LogicParse::Exp_beginContext* LogicParse::exp_begin() {
  Exp_beginContext *_localctx = _tracker.createInstance<Exp_beginContext>(_ctx, getState());
  enterRule(_localctx, 4, LogicParse::RuleExp_begin);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(34);
    match(LogicParse::EXP_BEGIN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Exp_endContext ------------------------------------------------------------------

LogicParse::Exp_endContext::Exp_endContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogicParse::Exp_endContext::EXP_END() {
  return getToken(LogicParse::EXP_END, 0);
}


size_t LogicParse::Exp_endContext::getRuleIndex() const {
  return LogicParse::RuleExp_end;
}

void LogicParse::Exp_endContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExp_end(this);
}

void LogicParse::Exp_endContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExp_end(this);
}


std::any LogicParse::Exp_endContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicParseVisitor*>(visitor))
    return parserVisitor->visitExp_end(this);
  else
    return visitor->visitChildren(this);
}

LogicParse::Exp_endContext* LogicParse::exp_end() {
  Exp_endContext *_localctx = _tracker.createInstance<Exp_endContext>(_ctx, getState());
  enterRule(_localctx, 6, LogicParse::RuleExp_end);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(36);
    match(LogicParse::EXP_END);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StrictContext ------------------------------------------------------------------

LogicParse::StrictContext::StrictContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogicParse::Atomic_expressionContext* LogicParse::StrictContext::atomic_expression() {
  return getRuleContext<LogicParse::Atomic_expressionContext>(0);
}

LogicParse::Exp_beginContext* LogicParse::StrictContext::exp_begin() {
  return getRuleContext<LogicParse::Exp_beginContext>(0);
}

LogicParse::Negation_operatorContext* LogicParse::StrictContext::negation_operator() {
  return getRuleContext<LogicParse::Negation_operatorContext>(0);
}

std::vector<LogicParse::StrictContext *> LogicParse::StrictContext::strict() {
  return getRuleContexts<LogicParse::StrictContext>();
}

LogicParse::StrictContext* LogicParse::StrictContext::strict(size_t i) {
  return getRuleContext<LogicParse::StrictContext>(i);
}

LogicParse::Exp_endContext* LogicParse::StrictContext::exp_end() {
  return getRuleContext<LogicParse::Exp_endContext>(0);
}

LogicParse::And_operatorContext* LogicParse::StrictContext::and_operator() {
  return getRuleContext<LogicParse::And_operatorContext>(0);
}

LogicParse::Or_operatorContext* LogicParse::StrictContext::or_operator() {
  return getRuleContext<LogicParse::Or_operatorContext>(0);
}

LogicParse::Implies_operatorContext* LogicParse::StrictContext::implies_operator() {
  return getRuleContext<LogicParse::Implies_operatorContext>(0);
}

LogicParse::Iff_operatorContext* LogicParse::StrictContext::iff_operator() {
  return getRuleContext<LogicParse::Iff_operatorContext>(0);
}


size_t LogicParse::StrictContext::getRuleIndex() const {
  return LogicParse::RuleStrict;
}

void LogicParse::StrictContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStrict(this);
}

void LogicParse::StrictContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStrict(this);
}


std::any LogicParse::StrictContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicParseVisitor*>(visitor))
    return parserVisitor->visitStrict(this);
  else
    return visitor->visitChildren(this);
}

LogicParse::StrictContext* LogicParse::strict() {
  StrictContext *_localctx = _tracker.createInstance<StrictContext>(_ctx, getState());
  enterRule(_localctx, 8, LogicParse::RuleStrict);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(68);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(38);
      atomic_expression();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(39);
      exp_begin();
      setState(40);
      negation_operator();
      setState(41);
      strict();
      setState(42);
      exp_end();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(44);
      exp_begin();
      setState(45);
      strict();
      setState(46);
      and_operator();
      setState(47);
      strict();
      setState(48);
      exp_end();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(50);
      exp_begin();
      setState(51);
      strict();
      setState(52);
      or_operator();
      setState(53);
      strict();
      setState(54);
      exp_end();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(56);
      exp_begin();
      setState(57);
      strict();
      setState(58);
      implies_operator();
      setState(59);
      strict();
      setState(60);
      exp_end();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(62);
      exp_begin();
      setState(63);
      strict();
      setState(64);
      iff_operator();
      setState(65);
      strict();
      setState(66);
      exp_end();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Negation_operatorContext ------------------------------------------------------------------

LogicParse::Negation_operatorContext::Negation_operatorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogicParse::Negation_operatorContext::NEGATION() {
  return getToken(LogicParse::NEGATION, 0);
}


size_t LogicParse::Negation_operatorContext::getRuleIndex() const {
  return LogicParse::RuleNegation_operator;
}

void LogicParse::Negation_operatorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNegation_operator(this);
}

void LogicParse::Negation_operatorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNegation_operator(this);
}


std::any LogicParse::Negation_operatorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicParseVisitor*>(visitor))
    return parserVisitor->visitNegation_operator(this);
  else
    return visitor->visitChildren(this);
}

LogicParse::Negation_operatorContext* LogicParse::negation_operator() {
  Negation_operatorContext *_localctx = _tracker.createInstance<Negation_operatorContext>(_ctx, getState());
  enterRule(_localctx, 10, LogicParse::RuleNegation_operator);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(70);
    match(LogicParse::NEGATION);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- And_operatorContext ------------------------------------------------------------------

LogicParse::And_operatorContext::And_operatorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogicParse::And_operatorContext::AND() {
  return getToken(LogicParse::AND, 0);
}


size_t LogicParse::And_operatorContext::getRuleIndex() const {
  return LogicParse::RuleAnd_operator;
}

void LogicParse::And_operatorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAnd_operator(this);
}

void LogicParse::And_operatorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAnd_operator(this);
}


std::any LogicParse::And_operatorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicParseVisitor*>(visitor))
    return parserVisitor->visitAnd_operator(this);
  else
    return visitor->visitChildren(this);
}

LogicParse::And_operatorContext* LogicParse::and_operator() {
  And_operatorContext *_localctx = _tracker.createInstance<And_operatorContext>(_ctx, getState());
  enterRule(_localctx, 12, LogicParse::RuleAnd_operator);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(72);
    match(LogicParse::AND);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Or_operatorContext ------------------------------------------------------------------

LogicParse::Or_operatorContext::Or_operatorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogicParse::Or_operatorContext::OR() {
  return getToken(LogicParse::OR, 0);
}


size_t LogicParse::Or_operatorContext::getRuleIndex() const {
  return LogicParse::RuleOr_operator;
}

void LogicParse::Or_operatorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOr_operator(this);
}

void LogicParse::Or_operatorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOr_operator(this);
}


std::any LogicParse::Or_operatorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicParseVisitor*>(visitor))
    return parserVisitor->visitOr_operator(this);
  else
    return visitor->visitChildren(this);
}

LogicParse::Or_operatorContext* LogicParse::or_operator() {
  Or_operatorContext *_localctx = _tracker.createInstance<Or_operatorContext>(_ctx, getState());
  enterRule(_localctx, 14, LogicParse::RuleOr_operator);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(74);
    match(LogicParse::OR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Implies_operatorContext ------------------------------------------------------------------

LogicParse::Implies_operatorContext::Implies_operatorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogicParse::Implies_operatorContext::IMPLIES() {
  return getToken(LogicParse::IMPLIES, 0);
}


size_t LogicParse::Implies_operatorContext::getRuleIndex() const {
  return LogicParse::RuleImplies_operator;
}

void LogicParse::Implies_operatorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterImplies_operator(this);
}

void LogicParse::Implies_operatorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitImplies_operator(this);
}


std::any LogicParse::Implies_operatorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicParseVisitor*>(visitor))
    return parserVisitor->visitImplies_operator(this);
  else
    return visitor->visitChildren(this);
}

LogicParse::Implies_operatorContext* LogicParse::implies_operator() {
  Implies_operatorContext *_localctx = _tracker.createInstance<Implies_operatorContext>(_ctx, getState());
  enterRule(_localctx, 16, LogicParse::RuleImplies_operator);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(76);
    match(LogicParse::IMPLIES);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Iff_operatorContext ------------------------------------------------------------------

LogicParse::Iff_operatorContext::Iff_operatorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogicParse::Iff_operatorContext::IFF() {
  return getToken(LogicParse::IFF, 0);
}


size_t LogicParse::Iff_operatorContext::getRuleIndex() const {
  return LogicParse::RuleIff_operator;
}

void LogicParse::Iff_operatorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIff_operator(this);
}

void LogicParse::Iff_operatorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIff_operator(this);
}


std::any LogicParse::Iff_operatorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicParseVisitor*>(visitor))
    return parserVisitor->visitIff_operator(this);
  else
    return visitor->visitChildren(this);
}

LogicParse::Iff_operatorContext* LogicParse::iff_operator() {
  Iff_operatorContext *_localctx = _tracker.createInstance<Iff_operatorContext>(_ctx, getState());
  enterRule(_localctx, 18, LogicParse::RuleIff_operator);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(78);
    match(LogicParse::IFF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Negated_expressionContext ------------------------------------------------------------------

LogicParse::Negated_expressionContext::Negated_expressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogicParse::Atomic_expressionContext* LogicParse::Negated_expressionContext::atomic_expression() {
  return getRuleContext<LogicParse::Atomic_expressionContext>(0);
}

LogicParse::Exp_beginContext* LogicParse::Negated_expressionContext::exp_begin() {
  return getRuleContext<LogicParse::Exp_beginContext>(0);
}

LogicParse::RelaxedInnerExprContext* LogicParse::Negated_expressionContext::relaxedInnerExpr() {
  return getRuleContext<LogicParse::RelaxedInnerExprContext>(0);
}

LogicParse::Exp_endContext* LogicParse::Negated_expressionContext::exp_end() {
  return getRuleContext<LogicParse::Exp_endContext>(0);
}


size_t LogicParse::Negated_expressionContext::getRuleIndex() const {
  return LogicParse::RuleNegated_expression;
}

void LogicParse::Negated_expressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNegated_expression(this);
}

void LogicParse::Negated_expressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNegated_expression(this);
}


std::any LogicParse::Negated_expressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicParseVisitor*>(visitor))
    return parserVisitor->visitNegated_expression(this);
  else
    return visitor->visitChildren(this);
}

LogicParse::Negated_expressionContext* LogicParse::negated_expression() {
  Negated_expressionContext *_localctx = _tracker.createInstance<Negated_expressionContext>(_ctx, getState());
  enterRule(_localctx, 20, LogicParse::RuleNegated_expression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(85);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case LogicParse::VARIABLE: {
        enterOuterAlt(_localctx, 1);
        setState(80);
        atomic_expression();
        break;
      }

      case LogicParse::EXP_BEGIN: {
        enterOuterAlt(_localctx, 2);
        setState(81);
        exp_begin();
        setState(82);
        relaxedInnerExpr();
        setState(83);
        exp_end();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RelaxedContext ------------------------------------------------------------------

LogicParse::RelaxedContext::RelaxedContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogicParse::Negation_operatorContext* LogicParse::RelaxedContext::negation_operator() {
  return getRuleContext<LogicParse::Negation_operatorContext>(0);
}

LogicParse::Negated_expressionContext* LogicParse::RelaxedContext::negated_expression() {
  return getRuleContext<LogicParse::Negated_expressionContext>(0);
}

LogicParse::Exp_beginContext* LogicParse::RelaxedContext::exp_begin() {
  return getRuleContext<LogicParse::Exp_beginContext>(0);
}

LogicParse::RelaxedInnerExprContext* LogicParse::RelaxedContext::relaxedInnerExpr() {
  return getRuleContext<LogicParse::RelaxedInnerExprContext>(0);
}

LogicParse::Exp_endContext* LogicParse::RelaxedContext::exp_end() {
  return getRuleContext<LogicParse::Exp_endContext>(0);
}

LogicParse::Atomic_expressionContext* LogicParse::RelaxedContext::atomic_expression() {
  return getRuleContext<LogicParse::Atomic_expressionContext>(0);
}

std::vector<LogicParse::RelaxedContext *> LogicParse::RelaxedContext::relaxed() {
  return getRuleContexts<LogicParse::RelaxedContext>();
}

LogicParse::RelaxedContext* LogicParse::RelaxedContext::relaxed(size_t i) {
  return getRuleContext<LogicParse::RelaxedContext>(i);
}

LogicParse::And_operatorContext* LogicParse::RelaxedContext::and_operator() {
  return getRuleContext<LogicParse::And_operatorContext>(0);
}

LogicParse::Or_operatorContext* LogicParse::RelaxedContext::or_operator() {
  return getRuleContext<LogicParse::Or_operatorContext>(0);
}

LogicParse::Implies_operatorContext* LogicParse::RelaxedContext::implies_operator() {
  return getRuleContext<LogicParse::Implies_operatorContext>(0);
}

LogicParse::Iff_operatorContext* LogicParse::RelaxedContext::iff_operator() {
  return getRuleContext<LogicParse::Iff_operatorContext>(0);
}


size_t LogicParse::RelaxedContext::getRuleIndex() const {
  return LogicParse::RuleRelaxed;
}

void LogicParse::RelaxedContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRelaxed(this);
}

void LogicParse::RelaxedContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRelaxed(this);
}


std::any LogicParse::RelaxedContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicParseVisitor*>(visitor))
    return parserVisitor->visitRelaxed(this);
  else
    return visitor->visitChildren(this);
}


LogicParse::RelaxedContext* LogicParse::relaxed() {
   return relaxed(0);
}

LogicParse::RelaxedContext* LogicParse::relaxed(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  LogicParse::RelaxedContext *_localctx = _tracker.createInstance<RelaxedContext>(_ctx, parentState);
  LogicParse::RelaxedContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 22;
  enterRecursionRule(_localctx, 22, LogicParse::RuleRelaxed, precedence);

    

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(96);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case LogicParse::NEGATION: {
        setState(88);
        negation_operator();
        setState(89);
        negated_expression();
        break;
      }

      case LogicParse::EXP_BEGIN: {
        setState(91);
        exp_begin();
        setState(92);
        relaxedInnerExpr();
        setState(93);
        exp_end();
        break;
      }

      case LogicParse::VARIABLE: {
        setState(95);
        atomic_expression();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(116);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(114);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
        case 1: {
          _localctx = _tracker.createInstance<RelaxedContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleRelaxed);
          setState(98);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(99);
          and_operator();
          setState(100);
          relaxed(6);
          break;
        }

        case 2: {
          _localctx = _tracker.createInstance<RelaxedContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleRelaxed);
          setState(102);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(103);
          or_operator();
          setState(104);
          relaxed(5);
          break;
        }

        case 3: {
          _localctx = _tracker.createInstance<RelaxedContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleRelaxed);
          setState(106);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(107);
          implies_operator();
          setState(108);
          relaxed(4);
          break;
        }

        case 4: {
          _localctx = _tracker.createInstance<RelaxedContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleRelaxed);
          setState(110);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(111);
          iff_operator();
          setState(112);
          relaxed(3);
          break;
        }

        default:
          break;
        } 
      }
      setState(118);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- RelaxedInnerExprContext ------------------------------------------------------------------

LogicParse::RelaxedInnerExprContext::RelaxedInnerExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogicParse::RelaxedInnerExprContext::getRuleIndex() const {
  return LogicParse::RuleRelaxedInnerExpr;
}

void LogicParse::RelaxedInnerExprContext::copyFrom(RelaxedInnerExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- FurtherNestedExprContext ------------------------------------------------------------------

LogicParse::Exp_beginContext* LogicParse::FurtherNestedExprContext::exp_begin() {
  return getRuleContext<LogicParse::Exp_beginContext>(0);
}

LogicParse::RelaxedInnerExprContext* LogicParse::FurtherNestedExprContext::relaxedInnerExpr() {
  return getRuleContext<LogicParse::RelaxedInnerExprContext>(0);
}

LogicParse::Exp_endContext* LogicParse::FurtherNestedExprContext::exp_end() {
  return getRuleContext<LogicParse::Exp_endContext>(0);
}

LogicParse::FurtherNestedExprContext::FurtherNestedExprContext(RelaxedInnerExprContext *ctx) { copyFrom(ctx); }

void LogicParse::FurtherNestedExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFurtherNestedExpr(this);
}
void LogicParse::FurtherNestedExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFurtherNestedExpr(this);
}

std::any LogicParse::FurtherNestedExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicParseVisitor*>(visitor))
    return parserVisitor->visitFurtherNestedExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NestedAndExprContext ------------------------------------------------------------------

std::vector<LogicParse::RelaxedContext *> LogicParse::NestedAndExprContext::relaxed() {
  return getRuleContexts<LogicParse::RelaxedContext>();
}

LogicParse::RelaxedContext* LogicParse::NestedAndExprContext::relaxed(size_t i) {
  return getRuleContext<LogicParse::RelaxedContext>(i);
}

LogicParse::And_operatorContext* LogicParse::NestedAndExprContext::and_operator() {
  return getRuleContext<LogicParse::And_operatorContext>(0);
}

LogicParse::NestedAndExprContext::NestedAndExprContext(RelaxedInnerExprContext *ctx) { copyFrom(ctx); }

void LogicParse::NestedAndExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNestedAndExpr(this);
}
void LogicParse::NestedAndExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNestedAndExpr(this);
}

std::any LogicParse::NestedAndExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicParseVisitor*>(visitor))
    return parserVisitor->visitNestedAndExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NestedOrExprContext ------------------------------------------------------------------

std::vector<LogicParse::RelaxedContext *> LogicParse::NestedOrExprContext::relaxed() {
  return getRuleContexts<LogicParse::RelaxedContext>();
}

LogicParse::RelaxedContext* LogicParse::NestedOrExprContext::relaxed(size_t i) {
  return getRuleContext<LogicParse::RelaxedContext>(i);
}

LogicParse::Or_operatorContext* LogicParse::NestedOrExprContext::or_operator() {
  return getRuleContext<LogicParse::Or_operatorContext>(0);
}

LogicParse::NestedOrExprContext::NestedOrExprContext(RelaxedInnerExprContext *ctx) { copyFrom(ctx); }

void LogicParse::NestedOrExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNestedOrExpr(this);
}
void LogicParse::NestedOrExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNestedOrExpr(this);
}

std::any LogicParse::NestedOrExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicParseVisitor*>(visitor))
    return parserVisitor->visitNestedOrExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NestedImpliesExprContext ------------------------------------------------------------------

std::vector<LogicParse::RelaxedContext *> LogicParse::NestedImpliesExprContext::relaxed() {
  return getRuleContexts<LogicParse::RelaxedContext>();
}

LogicParse::RelaxedContext* LogicParse::NestedImpliesExprContext::relaxed(size_t i) {
  return getRuleContext<LogicParse::RelaxedContext>(i);
}

LogicParse::Implies_operatorContext* LogicParse::NestedImpliesExprContext::implies_operator() {
  return getRuleContext<LogicParse::Implies_operatorContext>(0);
}

LogicParse::NestedImpliesExprContext::NestedImpliesExprContext(RelaxedInnerExprContext *ctx) { copyFrom(ctx); }

void LogicParse::NestedImpliesExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNestedImpliesExpr(this);
}
void LogicParse::NestedImpliesExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNestedImpliesExpr(this);
}

std::any LogicParse::NestedImpliesExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicParseVisitor*>(visitor))
    return parserVisitor->visitNestedImpliesExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NestedIffExprContext ------------------------------------------------------------------

std::vector<LogicParse::RelaxedContext *> LogicParse::NestedIffExprContext::relaxed() {
  return getRuleContexts<LogicParse::RelaxedContext>();
}

LogicParse::RelaxedContext* LogicParse::NestedIffExprContext::relaxed(size_t i) {
  return getRuleContext<LogicParse::RelaxedContext>(i);
}

LogicParse::Iff_operatorContext* LogicParse::NestedIffExprContext::iff_operator() {
  return getRuleContext<LogicParse::Iff_operatorContext>(0);
}

LogicParse::NestedIffExprContext::NestedIffExprContext(RelaxedInnerExprContext *ctx) { copyFrom(ctx); }

void LogicParse::NestedIffExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNestedIffExpr(this);
}
void LogicParse::NestedIffExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNestedIffExpr(this);
}

std::any LogicParse::NestedIffExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicParseVisitor*>(visitor))
    return parserVisitor->visitNestedIffExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NestedNotExprContext ------------------------------------------------------------------

LogicParse::Negation_operatorContext* LogicParse::NestedNotExprContext::negation_operator() {
  return getRuleContext<LogicParse::Negation_operatorContext>(0);
}

LogicParse::RelaxedContext* LogicParse::NestedNotExprContext::relaxed() {
  return getRuleContext<LogicParse::RelaxedContext>(0);
}

LogicParse::NestedNotExprContext::NestedNotExprContext(RelaxedInnerExprContext *ctx) { copyFrom(ctx); }

void LogicParse::NestedNotExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNestedNotExpr(this);
}
void LogicParse::NestedNotExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LogicParseListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNestedNotExpr(this);
}

std::any LogicParse::NestedNotExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicParseVisitor*>(visitor))
    return parserVisitor->visitNestedNotExpr(this);
  else
    return visitor->visitChildren(this);
}
LogicParse::RelaxedInnerExprContext* LogicParse::relaxedInnerExpr() {
  RelaxedInnerExprContext *_localctx = _tracker.createInstance<RelaxedInnerExprContext>(_ctx, getState());
  enterRule(_localctx, 24, LogicParse::RuleRelaxedInnerExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(142);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<LogicParse::NestedAndExprContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(119);
      relaxed(0);
      setState(120);
      and_operator();
      setState(121);
      relaxed(0);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<LogicParse::NestedOrExprContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(123);
      relaxed(0);
      setState(124);
      or_operator();
      setState(125);
      relaxed(0);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<LogicParse::NestedImpliesExprContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(127);
      relaxed(0);
      setState(128);
      implies_operator();
      setState(129);
      relaxed(0);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<LogicParse::NestedIffExprContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(131);
      relaxed(0);
      setState(132);
      iff_operator();
      setState(133);
      relaxed(0);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<LogicParse::NestedNotExprContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(135);
      negation_operator();
      setState(136);
      relaxed(0);
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<LogicParse::FurtherNestedExprContext>(_localctx);
      enterOuterAlt(_localctx, 6);
      setState(138);
      exp_begin();
      setState(139);
      relaxedInnerExpr();
      setState(140);
      exp_end();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool LogicParse::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 11: return relaxedSempred(antlrcpp::downCast<RelaxedContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool LogicParse::relaxedSempred(RelaxedContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 6);
    case 1: return precpred(_ctx, 5);
    case 2: return precpred(_ctx, 4);
    case 3: return precpred(_ctx, 3);

  default:
    break;
  }
  return true;
}

void LogicParse::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  logicparseParserInitialize();
#else
  ::antlr4::internal::call_once(logicparseParserOnceFlag, logicparseParserInitialize);
#endif
}
