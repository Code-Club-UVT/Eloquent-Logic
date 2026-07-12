
// Generated from LogicLex.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  LogicLex : public antlr4::Lexer {
public:
  enum {
    EXP_BEGIN = 1, EXP_END = 2, NEGATION = 3, AND = 4, OR = 5, IMPLIES = 6, 
    IFF = 7, VARIABLE = 8, WS = 9
  };

  explicit LogicLex(antlr4::CharStream *input);

  ~LogicLex() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

