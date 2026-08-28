add_library(satutils STATIC
        common.h
        mapping_generator.cpp
        mapping_generator.hpp
        mapper_listener.cpp
        mapper_listener.hpp
        sat_listener.cpp
        sat_listener.hpp)
target_link_libraries(satutils PRIVATE libLogicTransformations eloquent_logic_parser)
target_link_libraries(satutils PUBLIC liblogicCommon)