cmake_minimum_required(VERSION 3.16)

project(LibLogic VERSION 0.1 LANGUAGES CXX)

# Aceste setări se aplică la TOATE executabilele create mai jos (și LibLogic și my_tests)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Căutăm TOATE componentele Qt6 de care avem nevoie într-o singură linie
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Test)

set(PROJECT_SOURCES
        main.cpp
)

# ==========================================
# EXECUTABILUL 1: APLICAȚIA PRINCIPALĂ
# ==========================================
qt_add_executable(LibLogic
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        logiceditorwindow.h logiceditorwindow.cpp
        logiclineedit.h logiclineedit.cpp
        truthtableviewer.h truthtableviewer.cpp
        edge.h edge.cpp
        node.h node.cpp
        syntactictreeviewer.h syntactictreeviewer.cpp
        agentconnection.h agentconnection.cpp
        # AM ȘTERS FIȘIERELE DE TEST DE AICI! Nu au ce căuta în aplicație.
)

target_link_libraries(LibLogic PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network)

set_target_properties(LibLogic PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS LibLogic
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(LibLogic)


# ==========================================
# EXECUTABILUL 2: TESTELE (my_tests)
# ==========================================
# Aici spunem CMake-ului să creeze un program complet separat doar pentru teste
# IMPORTANT: Schimbă "tests.cpp" cu "test_main.cpp" dacă așa ai numit fișierul .cpp de test
# ==========================================
# EXECUTABILUL 2: TESTELE (my_tests)
# ==========================================
# EXECUTABILUL 2: TESTELE (my_tests)
# ==========================================
add_executable(my_tests
    tests.h test_main.cpp  # <-- ATENȚIE AICI: folosește test_main.cpp (cum se numește fișierul tău real)
    logiceditorwindow.h logiceditorwindow.cpp
    logiclineedit.h logiclineedit.cpp
    truthtableviewer.h truthtableviewer.cpp
    syntactictreeviewer.h syntactictreeviewer.cpp
    agentconnection.h agentconnection.cpp
    edge.h edge.cpp
    node.h node.cpp
)

target_link_libraries(my_tests PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network Qt6::Test)