add_library(liblogicTruthTable
        truth_table.cpp
        truth_table.hpp
        truth_table_listener_t.cpp
        truth_table_listener_t.hpp
        unsupported_operator_error.cpp
        unsupported_operator_error.hpp
        unbound_variable_error.cpp
        unbound_variable_error.hpp)
add_dependencies(liblogicTruthTable liblogicCommon)
add_dependencies(liblogicTruthTable eloquent_logic_parser)
target_link_libraries(liblogicTruthTable PUBLIC liblogicCommon)
target_link_libraries(liblogicTruthTable PUBLIC eloquent_logic_parser)
target_include_directories(liblogicTruthTable PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)