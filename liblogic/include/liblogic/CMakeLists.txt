include(GenerateExportHeader)

generate_export_header(LibLogic)

target_sources(eloquent_logic
            PUBLIC
                FILE_SET public_headers
                TYPE HEADERS
                FILES
                    interpretation_generator.h
                    liblogic.h
                    ${CMAKE_CURRENT_BINARY_DIR}/liblogic_export.h
            BASE_DIRS liblogic
)