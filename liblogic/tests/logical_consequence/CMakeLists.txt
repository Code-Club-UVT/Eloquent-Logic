add_executable(logical_consequence_test logical_consequence_tests.cpp)
target_link_libraries(logical_consequence_test PRIVATE gtest_main gmock)
target_link_libraries(logical_consequence_test PRIVATE liblogic_logical_consequence
        eloquent_logic_parser liblogic_test_common)
# liblogic_logical_consequence, like libLogicTransformations' own tests, needs
# its own headers plus its transitive dependencies' headers added explicitly:
# none of these export headers via target_include_directories/FILE_SET (see
# tests/transformations/CMakeLists.txt and logic_agent/CMakeLists.txt for the
# same reasoning).
target_include_directories(logical_consequence_test PRIVATE
        ${PROJECT_SOURCE_DIR}/src/logical_consequence
        ${PROJECT_SOURCE_DIR}/src/transformations
        ${PROJECT_SOURCE_DIR}/src/sat/utilities
)
include(GoogleTest)
gtest_discover_tests(logical_consequence_test)
