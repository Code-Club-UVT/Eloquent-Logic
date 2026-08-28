add_library(liblogic_logical_consequence logical_consequence.cpp logical_consequence.hpp
        logical_consequence_listener_t.cpp
        logical_consequence_listener_t.hpp
        incompatible_top_level_operator.cpp
        incompatible_top_level_operator.hpp)
target_link_libraries(liblogic_logical_consequence PRIVATE libLogicTransformations
        resolution_naive_first_fit
        dp_naive
        dpll_mcl
        dpll_rand
        satutils)