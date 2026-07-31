
add_library(eloquent_logic_parser cursor.cpp
        lexeme.cpp
        lexeme.hpp
        syntax_tree.cpp
        syntax_tree.hpp
        lexer.cpp
        lexer.hpp
        lexer_listener_t.cpp
        lexer_listener_t.hpp
        node.cpp
        parser_listener_t.cpp
        parser_listener_t.hpp
        strict_parser.cpp
        strict_parser.hpp
        relaxed_parser.cpp
        relaxed_parser.hpp
)
find_package(cppcommon)

# Then, link your executable or library with the corresponding package targets:

target_link_libraries(eloquent_logic_parser PUBLIC cppcommon::cppcommon)
target_link_libraries(eloquent_logic_parser PUBLIC liblogicCommon)
target_compile_features(eloquent_logic_parser PRIVATE cxx_std_23)
target_compile_options(eloquent_logic_parser PRIVATE -Wall -Wextra)
target_sources(eloquent_logic_parser
        PUBLIC
        FILE_SET sanitizer_headers
        TYPE HEADERS
        FILES
        cursor.h
        node.h

)