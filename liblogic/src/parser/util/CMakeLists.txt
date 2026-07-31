
add_library(eloquent_logic_parser cursor.cpp
        lexeme.cpp
        lexeme.hpp
        syntax_tree.cpp
        syntax_tree.hpp
        lexer.cpp
        lexer.hpp
        lexer_listener_t.cpp
        lexer_listener_t.hpp
)
add_dependencies(eloquent_logic_parser treelib)
target_include_directories(eloquent_logic_parser PRIVATE ${ANTLR4_INCLUDE_DIR})
target_link_libraries(eloquent_logic_parser PRIVATE treelib antlr4_static liblogicCommon)
target_compile_features(eloquent_logic_parser PRIVATE cxx_std_23)
target_compile_options(eloquent_logic_parser PRIVATE -Wall -Wextra)
target_sources(eloquent_logic_parser
        PUBLIC
        FILE_SET sanitizer_headers
        TYPE HEADERS
        FILES
        cursor.h
        execution_stack.h
        interpretation.h

)