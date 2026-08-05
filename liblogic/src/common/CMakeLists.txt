add_library(liblogicCommon
        feedback_listener.cpp
        hash_combine.hpp
)
find_package(nlohmann_json REQUIRED)
target_compile_options(liblogicCommon PUBLIC -Wall -Wextra)
target_sources(liblogicCommon PUBLIC FILE_SET HEADERS FILES feedback_listener.hpp)
target_link_libraries(liblogicCommon PUBLIC)