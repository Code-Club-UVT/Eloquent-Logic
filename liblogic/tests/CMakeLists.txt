add_executable(liblogic_tests main.cpp
        truth-machine.cpp
        transformations.cpp
        traversal_tests.cpp)


find_package(antlr4-runtime REQUIRED)

target_link_libraries(liblogic_tests PRIVATE eloquent_logic eloquent_logic_parser)
target_include_directories(liblogic_tests PRIVATE ${ANTLR4_INCLUDE_DIR})

target_sources(liblogic_tests PRIVATE
        FILE_SET test_headers
        TYPE HEADERS
        FILES headers/test_wrapper_functions.h
        BASE_DIRS headers)

add_subdirectory(headers)

target_compile_definitions(liblogic_tests PRIVATE GTEST_COLOR=yes)


include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
if(WINDOWS)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()
FetchContent_MakeAvailable(googletest)



target_link_libraries(liblogic_tests PRIVATE gtest_main gmock)
include(GoogleTest)
gtest_discover_tests(liblogic_tests)