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
)
target_link_libraries(logic_agent PRIVATE eloquent_logic)

find_package(nlohmann_json)
target_link_libraries(logic_agent PRIVATE nlohmann_json::nlohmann_json)