add_library(memutils STATIC
        common.h
        mapping_generator.cpp
        mapping_generator.hpp
        mapper_listener.cpp
        mapper_listener.hpp
        sat_listener.cpp
        sat_listener.hpp)
target_link_libraries(memutils PRIVATE libLogicTransformations eloquent_logic_parser)
target_link_libraries(memutils PUBLIC liblogicCommon)