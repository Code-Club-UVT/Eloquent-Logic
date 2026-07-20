add_library(liblogicCommon steppable.cpp
        feedback_base.cpp
        steppable_result.cpp
)
target_compile_options(liblogicCommon PUBLIC -Wall -Wextra)
target_sources(liblogicCommon PUBLIC FILE_SET HEADERS FILES steppable.hpp steppable_result.hpp feedback_base.hpp)