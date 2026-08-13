target_sources(eloquent_logic_parser
        PRIVATE
        LogicLex.cpp LogicParse.cpp LogicParseBaseListener.cpp LogicParseBaseVisitor.cpp LogicParseListener.cpp LogicParseVisitor.cpp)
target_sources(eloquent_logic_parser
        PUBLIC
        FILE_SET generated_parser_headers
        TYPE HEADERS
        FILES
        LogicLex.h
        LogicParse.h
        LogicParseBaseListener.h
        LogicParseBaseVisitor.h

)