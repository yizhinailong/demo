cmake_minimum_required(VERSION 3.16)

project(Demo6ContextObjects VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Quick
    Qml
    QuickControls2
)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appDemo6ContextObjects
    main.cpp
)

qt_add_qml_module(appDemo6ContextObjects
    URI Demo6ContextObjects
    QML_FILES
        Main.qml
    SOURCES
        propertywrapper.hpp
        propertywrapper.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appDemo6ContextObjects PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appDemo6ContextObjects
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appDemo6ContextObjects
    PRIVATE
        Qt6::Quick
        Qt6::Qml
        Qt6::QuickControls2
)

include(GNUInstallDirs)
install(TARGETS appDemo6ContextObjects
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
