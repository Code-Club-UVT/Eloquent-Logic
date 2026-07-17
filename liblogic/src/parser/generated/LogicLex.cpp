
// Generated from LogicLex.g4 by ANTLR 4.13.2


#include "LogicLex.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct LogicLexStaticData final {
  LogicLexStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  LogicLexStaticData(const LogicLexStaticData&) = delete;
  LogicLexStaticData(LogicLexStaticData&&) = delete;
  LogicLexStaticData& operator=(const LogicLexStaticData&) = delete;
  LogicLexStaticData& operator=(LogicLexStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag logiclexLexerOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<LogicLexStaticData> logiclexLexerStaticData = nullptr;

void logiclexLexerInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (logiclexLexerStaticData != nullptr) {
    return;
  }
#else
  assert(logiclexLexerStaticData == nullptr);
#endif
  auto staticData = std::make_unique<LogicLexStaticData>(
    std::vector<std::string>{
      "EXP_BEGIN", "EXP_END", "NEGATION", "AND", "OR", "IMPLIES", "IFF", 
      "VARIABLE", "WS"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
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
  	4,0,9,92,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,
  	2,7,7,7,2,8,7,8,1,0,1,0,1,1,1,1,1,2,1,2,1,2,1,2,1,2,1,2,1,3,1,3,1,3,1,
  	3,1,3,1,3,1,3,1,3,1,4,1,4,1,4,1,4,1,4,1,4,1,5,1,5,1,5,1,5,1,5,1,5,1,5,
  	1,5,1,5,1,5,1,6,1,6,1,6,1,6,1,6,1,6,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,
  	7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,4,7,79,8,7,11,7,12,7,80,1,7,
  	3,7,84,8,7,1,8,4,8,87,8,8,11,8,12,8,88,1,8,1,8,0,0,9,1,1,3,2,5,3,7,4,
  	9,5,11,6,13,7,15,8,17,9,1,0,3,2,0,65,90,97,122,1,0,48,57,3,0,9,10,12,
  	13,32,32,97,0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,
  	0,0,11,1,0,0,0,0,13,1,0,0,0,0,15,1,0,0,0,0,17,1,0,0,0,1,19,1,0,0,0,3,
  	21,1,0,0,0,5,23,1,0,0,0,7,29,1,0,0,0,9,37,1,0,0,0,11,43,1,0,0,0,13,53,
  	1,0,0,0,15,83,1,0,0,0,17,86,1,0,0,0,19,20,5,40,0,0,20,2,1,0,0,0,21,22,
  	5,41,0,0,22,4,1,0,0,0,23,24,5,92,0,0,24,25,5,92,0,0,25,26,5,110,0,0,26,
  	27,5,101,0,0,27,28,5,103,0,0,28,6,1,0,0,0,29,30,5,92,0,0,30,31,5,92,0,
  	0,31,32,5,119,0,0,32,33,5,101,0,0,33,34,5,100,0,0,34,35,5,103,0,0,35,
  	36,5,101,0,0,36,8,1,0,0,0,37,38,5,92,0,0,38,39,5,92,0,0,39,40,5,118,0,
  	0,40,41,5,101,0,0,41,42,5,101,0,0,42,10,1,0,0,0,43,44,5,92,0,0,44,45,
  	5,92,0,0,45,46,5,105,0,0,46,47,5,109,0,0,47,48,5,112,0,0,48,49,5,108,
  	0,0,49,50,5,105,0,0,50,51,5,101,0,0,51,52,5,115,0,0,52,12,1,0,0,0,53,
  	54,5,92,0,0,54,55,5,92,0,0,55,56,5,105,0,0,56,57,5,102,0,0,57,58,5,102,
  	0,0,58,14,1,0,0,0,59,60,5,92,0,0,60,61,5,92,0,0,61,62,5,116,0,0,62,63,
  	5,111,0,0,63,84,5,112,0,0,64,65,5,92,0,0,65,66,5,92,0,0,66,67,5,98,0,
  	0,67,68,5,111,0,0,68,84,5,116,0,0,69,84,7,0,0,0,70,71,7,0,0,0,71,72,5,
  	95,0,0,72,84,7,1,0,0,73,74,7,0,0,0,74,75,5,95,0,0,75,76,5,123,0,0,76,
  	78,1,0,0,0,77,79,7,1,0,0,78,77,1,0,0,0,79,80,1,0,0,0,80,78,1,0,0,0,80,
  	81,1,0,0,0,81,82,1,0,0,0,82,84,5,125,0,0,83,59,1,0,0,0,83,64,1,0,0,0,
  	83,69,1,0,0,0,83,70,1,0,0,0,83,73,1,0,0,0,84,16,1,0,0,0,85,87,7,2,0,0,
  	86,85,1,0,0,0,87,88,1,0,0,0,88,86,1,0,0,0,88,89,1,0,0,0,89,90,1,0,0,0,
  	90,91,6,8,0,0,91,18,1,0,0,0,4,0,80,83,88,1,6,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  logiclexLexerStaticData = std::move(staticData);
}

}

LogicLex::LogicLex(CharStream *input) : Lexer(input) {
  LogicLex::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *logiclexLexerStaticData->atn, logiclexLexerStaticData->decisionToDFA, logiclexLexerStaticData->sharedContextCache);
}

LogicLex::~LogicLex() {
  delete _interpreter;
}

std::string LogicLex::getGrammarFileName() const {
  return "LogicLex.g4";
}

const std::vector<std::string>& LogicLex::getRuleNames() const {
  return logiclexLexerStaticData->ruleNames;
}

const std::vector<std::string>& LogicLex::getChannelNames() const {
  return logiclexLexerStaticData->channelNames;
}

const std::vector<std::string>& LogicLex::getModeNames() const {
  return logiclexLexerStaticData->modeNames;
}

const dfa::Vocabulary& LogicLex::getVocabulary() const {
  return logiclexLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView LogicLex::getSerializedATN() const {
  return logiclexLexerStaticData->serializedATN;
}

const atn::ATN& LogicLex::getATN() const {
  return *logiclexLexerStaticData->atn;
}




void LogicLex::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  logiclexLexerInitialize();
#else
  ::antlr4::internal::call_once(logiclexLexerOnceFlag, logiclexLexerInitialize);
#endif
}
