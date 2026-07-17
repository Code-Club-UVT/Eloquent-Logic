add_library(treelib breath_first_traversal.cpp
        depth_first_traversal.cpp
        luke_tree_walker.cpp
        tree_builder.cpp)
target_sources(treelib
        PUBLIC
        FILE_SET tree_utilities
        TYPE HEADERS
        FILES
        breath_first_traversal.h
        depth_first_traversal.h
        tree_walker_strategy_base.h
        tree_walker.h
        tree_builder.h
        dictionary.h
)