add_executable(logic_agent_schema_test schema_tests.cpp)
target_link_libraries(logic_agent_schema_test PRIVATE gtest_main gmock)

find_package(nlohmann_json)
find_package(nlohmann_json_schema_validator)
target_link_libraries(logic_agent_schema_test PRIVATE
        nlohmann_json::nlohmann_json
        nlohmann_json_schema_validator
)

# The schema files are read from disk at test time (not embedded), so the
# tests need to know where docs/schema/ lives regardless of the build
# directory's location.
target_compile_definitions(logic_agent_schema_test PRIVATE
        SCHEMA_DIR="${PROJECT_SOURCE_DIR}/docs/schema"
)

include(GoogleTest)
gtest_discover_tests(logic_agent_schema_test)
