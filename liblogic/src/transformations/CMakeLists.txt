target_sources(eloquent_logic PRIVATE de_morgan.cpp double_negation.cpp equivalence_reduction.cpp implication_reduction.cpp inverter.cpp n-ization.cpp neutral_elements.cpp)
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
        n-ization.h
        neutral_elements.h
        tree_recognizer_base.h
)