add_library(heuristics HeuristicEntryDP.cpp HeuristicsDB_DP.cpp)
target_sources(heuristics PUBLIC FILE_SET HEADERS FILES HeuristicEntryDP.h HeuristicsDB_DP.h)
target_link_libraries(heuristics PRIVATE memutils)