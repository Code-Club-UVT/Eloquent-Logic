target_sources(eloquent_logic
        PUBLIC
        FILE_SET tree_startegies_headers
        TYPE HEADERS
        FILES
        breath_first_traversal.h
        depth_first_traversal.h
        tree_walker_strategy_base.h
)