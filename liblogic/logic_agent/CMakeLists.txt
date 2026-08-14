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
)
target_link_libraries(logic_agent PRIVATE eloquent_logic)

# src/sat/utilities (target memutils) doesn't export its headers via
# target_include_directories/FILE_SET the way eloquent_logic_parser does,
# so types/sat/convert.cpp needs an explicit include dir for common.h
# (Literal/Clause/ClauseSet). It's a header-only need: common.h declares no
# compiled symbols, so no target_link_libraries is required.
target_include_directories(logic_agent PRIVATE ${CMAKE_SOURCE_DIR}/src/sat/utilities)

find_package(nlohmann_json)
target_link_libraries(logic_agent PRIVATE nlohmann_json::nlohmann_json)