add_library(liblogicCommon INTERFACE steppable.cpp
        feedback_base.cpp
        steppable_result.cpp
)

target_sources(liblogicCommon PUBLIC FILE_SET HEADERS FILES steppable.hpp steppable_result.hpp feedback_base.hpp)