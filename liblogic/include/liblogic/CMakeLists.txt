include(GenerateExportHeader)

generate_export_header(LibLogic)

target_sources(eloquent_logic
            PUBLIC
                FILE_SET public_headers
                TYPE HEADERS
                FILES
                    interpretation_generator.h
                    dictionary.h
                    liblogic.h
                    tree_sanitizer.h
                    ${CMAKE_CURRENT_BINARY_DIR}/liblogic_export.h
            BASE_DIRS private
)
add_subdirectory(private)