target_sources(eloquent_logic
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LogicLex.cpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LogicParse.cpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LogicParseBaseListener.cpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LogicParseBaseVisitor.cpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LogicParseListener.cpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LogicParseVisitor.cpp>
        $<INSTALL_INTERFACE:LogicLex.cpp>
        $<INSTALL_INTERFACE:LogicLex.cpp>
        $<INSTALL_INTERFACE:LogicParse.cpp>
        $<INSTALL_INTERFACE:LogicParseBaseListener.cpp>
        $<INSTALL_INTERFACE:LogicParseBaseVisitor.cpp>
        $<INSTALL_INTERFACE:LogicParseListener.cpp>
        $<INSTALL_INTERFACE:LogicParseVisitor.cpp>

        PRIVATE
        LogicLex.cpp LogicParse.cpp LogicParseBaseListener.cpp LogicParseBaseVisitor.cpp LogicParseListener.cpp LogicParseVisitor.cpp )
target_sources(eloquent_logic
        PUBLIC
        FILE_SET generated_parser_headers
        TYPE HEADERS
        FILES
            LogicLex.h
            LogicParse.h
            LogicParseBaseListener.h
            LogicParseBaseVisitor.h

)
target_link_libraries(eloquent_logic PUBLIC antlr4_shared)