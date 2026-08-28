add_executable(transformations_test
        node_transformation_listener_tests.cpp
        transformation_runner_tests.cpp)
target_link_libraries(transformations_test PRIVATE gtest_main gmock)
target_link_libraries(transformations_test PRIVATE libLogicTransformations eloquent_logic_parser)
# libLogicTransformations' headers are only advertised as a FILE_SET on the
# top-level eloquent_logic target (they're installed as its public headers),
# so a consumer that links just libLogicTransformations (per CLAUDE.md: never
# link the whole library into a test) needs the include dir added explicitly.
target_include_directories(transformations_test PRIVATE ${PROJECT_SOURCE_DIR}/src/transformations)
# transformation_runner_tests.cpp exercises to_cnf's real-world symptom
# end-to-end via dpll_mcl/resolution; those targets (like the SAT utilities
# they share) don't export their headers via target_include_directories/
# FILE_SET the way eloquent_logic_parser does, so the include dirs below are
# needed explicitly (mirrors logic_agent/CMakeLists.txt's own setup).
target_link_libraries(transformations_test PRIVATE satutils dpll_mcl resolution_naive_first_fit)
target_include_directories(transformations_test PRIVATE
        ${PROJECT_SOURCE_DIR}/src/sat/utilities
        ${PROJECT_SOURCE_DIR}/src/sat/dpll_mcl
        ${PROJECT_SOURCE_DIR}/src/sat/resolution_naive_first_fit
)
include(GoogleTest)
gtest_discover_tests(transformations_test)
