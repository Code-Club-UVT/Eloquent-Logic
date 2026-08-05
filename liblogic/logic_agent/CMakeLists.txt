add_executable(logic_agent agent.cpp)
target_link_libraries(logic_agent PRIVATE eloquent_logic)

find_package(nlohmann_json)
target_link_libraries(logic_agent PRIVATE nlohmann_json::nlohmann_json)