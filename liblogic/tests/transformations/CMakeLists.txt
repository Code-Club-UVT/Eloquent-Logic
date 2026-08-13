add_executable(transformations_test
        node_transformation_listener_tests.cpp)
target_link_libraries(transformations_test PRIVATE gtest_main gmock)
target_link_libraries(transformations_test PRIVATE libLogicTransformations eloquent_logic_parser)
# libLogicTransformations' headers are only advertised as a FILE_SET on the
# top-level eloquent_logic target (they're installed as its public headers),
# so a consumer that links just libLogicTransformations (per CLAUDE.md: never
# link the whole library into a test) needs the include dir added explicitly.
target_include_directories(transformations_test PRIVATE ${PROJECT_SOURCE_DIR}/src/transformations)
include(GoogleTest)
gtest_discover_tests(transformations_test)
