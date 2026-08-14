add_executable(parser_test parser_tests.cpp
        strict_parser_tests.cpp
        relaxed_parser_tests.cpp
        lexer_tests.cpp
        node_tests.cpp)
target_link_libraries(parser_test PRIVATE gtest_main gmock)
target_link_libraries(parser_test PRIVATE eloquent_logic_parser)
include(GoogleTest)
gtest_discover_tests(parser_test)