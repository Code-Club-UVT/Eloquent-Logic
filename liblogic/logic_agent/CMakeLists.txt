add_executable(logic_agent agent.cpp
        types/core/uuid_t.hpp
        types/core/lexeme_t.hpp
        types/core/node_t.hpp
        types/core/convert.hpp
        types/core/convert.cpp
        types/parser/cursor_t.hpp
        types/parser/lexer_events.hpp
        types/parser/cursor_events.hpp
        types/parser/strict_parser_events.hpp
        types/parser/relaxed_parser_events.hpp
        types/truth_table/truth_table_events.hpp
        types/sat/common_t.hpp
        types/sat/mapper_events.hpp
        types/sat/sat_events.hpp
        types/sat/convert.hpp
        types/sat/convert.cpp
        types/transformations/node_transformation_events.hpp
        types/rpc_request.hpp
        types/rpc_request.cpp
        types/rpc_response.hpp
        types/rpc_response.cpp
        types/feedback_base.hpp
        rpc/rpc_exception.hpp
        rpc/event_sink.hpp
        rpc/formula_pipeline.hpp
        rpc/formula_pipeline.cpp
        rpc/streaming_lexer_listener.hpp
        rpc/streaming_lexer_listener.cpp
        rpc/streaming_relaxed_parser_listener.hpp
        rpc/streaming_relaxed_parser_listener.cpp
        rpc/streaming_strict_parser_listener.hpp
        rpc/streaming_strict_parser_listener.cpp
        rpc/streaming_truth_table_listener.hpp
        rpc/streaming_truth_table_listener.cpp
        rpc/streaming_transformation_listener.hpp
        rpc/streaming_transformation_listener.cpp
        rpc/streaming_mapper_listener.hpp
        rpc/streaming_mapper_listener.cpp
        rpc/streaming_sat_listener.hpp
        rpc/streaming_sat_listener.cpp
        rpc/methods/truth_table_method.hpp
        rpc/methods/truth_table_method.cpp
        rpc/methods/parse_method.hpp
        rpc/methods/parse_method.cpp
        rpc/methods/transform_method.hpp
        rpc/methods/transform_method.cpp
        rpc/methods/sat_method.hpp
        rpc/methods/sat_method.cpp
        rpc/methods/shutdown_method.hpp
        rpc/methods/shutdown_method.cpp
        rpc/dispatcher.hpp
        rpc/dispatcher.cpp
)
target_link_libraries(logic_agent PRIVATE eloquent_logic)

# src/sat/utilities (target memutils), and the individual solver targets
# below, don't export their headers via target_include_directories/FILE_SET
# the way eloquent_logic_parser does, so logic_agent needs explicit include
# dirs for their headers (common.h, mapping_generator.hpp, dp_naive.h, ...).
target_include_directories(logic_agent PRIVATE
        ${CMAKE_SOURCE_DIR}/src/sat/utilities
        ${CMAKE_SOURCE_DIR}/src/sat/dp_naive
        ${CMAKE_SOURCE_DIR}/src/sat/dpll_mcl
        ${CMAKE_SOURCE_DIR}/src/sat/dpll_rand
        ${CMAKE_SOURCE_DIR}/src/sat/resolution_naive_first_fit
)

# The "sat"/"transform" RPC methods call real (non-header-only) symbols from
# these targets (mapping_generator::gen_map, dp/dpll_mcl/dpll_rand/res,
# transformation_runner/cnf_recogniser). libLogicTransformations also
# exports its headers (transformation_runner.hpp, cnf_recogniser.hpp, ...)
# via a PUBLIC FILE_SET, so linking it needs no extra include dir above.
target_link_libraries(logic_agent PRIVATE
        memutils
        dp_naive
        dpll_mcl
        dpll_rand
        resolution_naive_first_fit
        libLogicTransformations
)

find_package(nlohmann_json)
target_link_libraries(logic_agent PRIVATE nlohmann_json::nlohmann_json)
