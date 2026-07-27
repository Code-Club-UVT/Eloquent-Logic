add_library(liblogicCommon steppable.cpp
        feedback_base.cpp
        steppable_result.cpp
        feedback_provider.h
)
find_package(nlohmann_json REQUIRED)
target_compile_options(liblogicCommon PUBLIC -Wall -Wextra)
target_sources(liblogicCommon PUBLIC FILE_SET HEADERS FILES steppable.hpp steppable_result.hpp feedback_base.hpp)
target_link_libraries(liblogicCommon PUBLIC nlohmann_json::nlohmann_json)