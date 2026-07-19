add_library(eloquent_logic liblogic.cpp)

add_dependencies(eloquent_logic treelib)
add_dependencies(eloquent_logic liblogicCommon)
target_link_libraries(eloquent_logic PUBLIC treelib liblogicCommon)

add_dependencies(eloquent_logic eloquent_logic_parser)
add_dependencies(eloquent_logic liblogicTruthTable)
target_link_libraries(eloquent_logic PUBLIC eloquent_logic_parser liblogicTruthTable)

add_subdirectory(truth_table)
add_subdirectory(transformations)

target_compile_options(eloquent_logic PRIVATE $<$<AND:$<CONFIG:Debug>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-g -Wall -Wextra -Werror -Warray-bounds -Wno-unused-parameter >$<$<AND:$<CONFIG:Release>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-O3>)



set_target_properties(eloquent_logic PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        EXPORT_NAME Logic
)
