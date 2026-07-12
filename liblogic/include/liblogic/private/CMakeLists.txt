target_sources(eloquent_logic
        PUBLIC
        FILE_SET private_headers
        TYPE HEADERS
        FILES
            cursor.h
            execution_stack.h
            interpretation.h
            operation_private.h
            transformations.h
            tree_builder.h
            tree_walker.h
        BASE_DIRS
            tree_strategies
            transformations
            operations
)
add_subdirectory(operations)
add_subdirectory(transformations)
add_subdirectory(tree_strategies)