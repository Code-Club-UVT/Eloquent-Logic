add_library(dp_naive dp_naive.cpp
        dp_naive.h)
target_link_libraries(dp_naive PRIVATE memutils heuristics)