add_library(liblogicTruthTable
        truth_table.cpp
        truth_table.hpp)
add_subdirectory(operations)
add_dependencies(liblogicTruthTable liblogicCommon)
target_link_libraries(liblogicTruthTable PRIVATE treelib liblogicCommon)

#TODO: remove this after transition to the new truth table engine is complete
target_link_libraries(liblogicTruthTable PRIVATE eloquent_logic_parser)