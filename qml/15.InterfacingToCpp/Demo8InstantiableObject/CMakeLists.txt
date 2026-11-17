cmake_minimum_required(VERSION 3.16)

project(Demo8InstantiableObject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Quick
    Qml
    QuickControls2
)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appDemo8InstantiableObject
    main.cpp
)

qt_add_qml_module(appDemo8InstantiableObject
    URI Demo8InstantiableObject
    QML_FILES
        Main.qml
        SOURCES movie.hpp movie.cpp
)


target_link_libraries(appDemo8InstantiableObject
    PRIVATE
        Qt6::Quick
        Qt6::Qml
        Qt6::QuickControls2
)

include(GNUInstallDirs)
install(TARGETS appDemo8InstantiableObject
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
