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

add_subdirectory(common)
add_subdirectory(parser)
add_subdirectory(truth_table)
add_subdirectory(transformations)
add_subdirectory(logical_consequence)
add_subdirectory(logic_agent)
