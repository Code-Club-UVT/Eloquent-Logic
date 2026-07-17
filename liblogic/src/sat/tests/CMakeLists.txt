add_executable(TestDP
        dp_naive.cpp
)
target_link_libraries(TestDP PRIVATE dp_naive)

add_executable(TestDPLL_RAND
        dpll_rand.cpp
)
target_link_libraries(TestDPLL_RAND PRIVATE dpll_rand)


add_executable(TestDPLL_MCL
        dpll_mcl.cpp
)
target_link_libraries(TestDPLL_MCL PRIVATE dpll_mcl)

add_executable(TestResolution
        resolution_naive_first_fit.cpp
)
target_link_libraries(TestResolution PRIVATE resolution_naive_first_fit)
target_include_directories(TestDP PRIVATE
        ../utilities
        ../dp_naive
        ../Heuristics
)

target_include_directories(TestDPLL_RAND PRIVATE
        ../utilities
        ../dpll_rand
)

target_include_directories(TestDPLL_MCL PRIVATE
        ../utilities
        ../dpll_mcl
)

target_include_directories(TestResolution PRIVATE
        ../utilities
        ../resolution_naive_first_fit
)

target_link_libraries(TestResolution PRIVATE
        GTest::gtest
        GTest::gtest_main
        memutils
)

target_link_libraries(TestDP PRIVATE
        GTest::gtest
        GTest::gtest_main
        memutils
)

target_link_libraries(TestDPLL_RAND PRIVATE
        GTest::gtest
        GTest::gtest_main
        memutils
)

target_link_libraries(TestDPLL_MCL PRIVATE
        GTest::gtest
        GTest::gtest_main
        memutils
)

include(GoogleTest)
gtest_discover_tests(TestDP)
gtest_discover_tests(TestDPLL_RAND)
gtest_discover_tests(TestDPLL_MCL)
gtest_discover_tests(TestResolution)

