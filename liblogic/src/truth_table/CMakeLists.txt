add_library(liblogicTruthTable
        truth_table.cpp
        truth_table.hpp
        truth_table_listener_t.cpp
        truth_table_listener_t.hpp
        unsupported_operator_error.cpp
        unsupported_operator_error.hpp)
add_dependencies(liblogicTruthTable liblogicCommon)
target_link_libraries(liblogicTruthTable PRIVATE liblogicCommon)
target_link_libraries(liblogicTruthTable PRIVATE eloquent_logic_parser)