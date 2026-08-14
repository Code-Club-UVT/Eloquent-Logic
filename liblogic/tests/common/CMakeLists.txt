add_library(liblogic_test_common
        token_builders.cpp
        token_builders.hpp)
target_link_libraries(liblogic_test_common PUBLIC eloquent_logic_parser)
target_include_directories(liblogic_test_common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
