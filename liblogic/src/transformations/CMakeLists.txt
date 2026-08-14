add_library(libLogicTransformations  de_morgan.cpp double_negation.cpp equivalence_reduction.cpp implication_reduction.cpp inverter.cpp neutral_elements.cpp
        tree_matcher.cpp
        tree_matcher.hpp
        transformation_runner.cpp
        transformation_runner.hpp
        logical_implication.cpp
        logical_implication.hpp
        distributions.cpp
        distributions.hpp
        node_transformation_listener_t.cpp
        node_transformation_listener_t.hpp
        cnf_recogniser.cpp
        cnf_recogniser.hpp
        dnf_recogniser.cpp
        dnf_recogniser.hpp
        absorption_transformation.cpp
        absorption_transformation.hpp
)
target_sources(libLogicTransformations
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
        tree_matcher.hpp
        transformation_runner.hpp
        logical_implication.hpp
        distributions.hpp
        node_transformation_listener_t.hpp
        cnf_recogniser.hpp
        dnf_recogniser.hpp
        absorption_transformation.hpp
)
target_link_libraries(libLogicTransformations PUBLIC liblogicCommon eloquent_logic_parser)