add_library(libLogicTransformations  de_morgan.cpp double_negation.cpp equivalence_reduction.cpp implication_reduction.cpp inverter.cpp neutral_elements.cpp
        tree_matcher.cpp
        tree_matcher.hpp
        transformation_runner.cpp
        transformation_runner.hpp
        logical_implication.cpp
        logical_implication.hpp
        and_distribution.cpp
        and_distribution.hpp
        node_transformation_listener_t.cpp
        node_transformation_listener_t.hpp)
target_sources(eloquent_logic
        PUBLIC
        FILE_SET transformation_headers
        TYPE HEADERS
        FILES
        de_morgan.h
        double_negation.h
        equivalence_reduction.h
        implication_reduction.h
        inverter.h
        neutral_elements.h
        tree_operation_base.h
)
target_link_libraries(libLogicTransformations PUBLIC liblogicCommon eloquent_logic_parser)