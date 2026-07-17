
add_library(eloquent_logic_parser tree_sanitizer.cpp cursor.cpp)
add_dependencies(eloquent_logic_parser treelib)
target_include_directories(eloquent_logic_parser PRIVATE ${ANTLR4_INCLUDE_DIR})
target_link_libraries(eloquent_logic_parser PRIVATE treelib antlr4_shared)

target_sources(eloquent_logic_parser
        PUBLIC
        FILE_SET sanitizer_headers
        TYPE HEADERS
        FILES
        tree_sanitizer.h
        cursor.h
        # TODO: move the execution stack from the parser. It shouldn't be bunched up with the AST making parts of LibLogic
        execution_stack.h
        interpretation.h

)