cmake_minimum_required(VERSION 3.25)

project(EloquentLogic VERSION 1.0.0 LANGUAGES CXX)



option(LIBLOGIC_BUILD_TESTS "Whether to build tests for LibLogic" ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src/common)
add_subdirectory(src/tree_utils)
add_subdirectory(src/parser)
add_subdirectory(src)
add_subdirectory(src/sat)

target_sources(eloquent_logic
        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
        include
        ${CMAKE_CURRENT_BINARY_DIR}/include)

add_library(Eloquent::Logic ALIAS eloquent_logic)




if(LIBLOGIC_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()
include(GNUInstallDirs)

install(TARGETS eloquent_logic eloquent_logic_parser treelib liblogicCommon liblogicTruthTable
        EXPORT eloquent_logic_targets
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        FILE_SET HEADERS
        FILE_SET generated_parser_headers
        FILE_SET sanitizer_headers
        FILE_SET tree_utilities
        FILE_SET transformation_headers
        FILE_SET operations_headers

)
install(EXPORT eloquent_logic_targets
        FILE eloquent_logic_targets.cmake
        NAMESPACE Eloquent::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/EloquentLogic
)
include(CMakePackageConfigHelpers)

set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/liblogic
        CACHE PATH "Location of header files" )
set(ELOQUENT_LOGIC_VERSION ${PROJECT_VERSION}
        CACHE PATH "Version of project" )
configure_package_config_file(cmake/EloquentLogicConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/EloquentLogicConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/EloquentLogic
        PATH_VARS INCLUDE_INSTALL_DIR ELOQUENT_LOGIC_VERSION)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/cmake/EloquentLogicConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
        ARCH_INDEPENDENT
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/EloquentLogicConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/EloquentLogicConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/EloquentLogic )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/EloquentLogicConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/EloquentLogicConfigVersion.cmake
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/cmake )