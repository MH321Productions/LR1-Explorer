project(LR1-Explorer-Qml)

qt_standard_project_setup()

find_package(Qt6 REQUIRED COMPONENTS Quick3D)

qt_add_library(qmlModule STATIC)
target_link_libraries(qmlModule PRIVATE Qt6::Quick3D)
target_include_directories(qmlModule PRIVATE ../include)
target_include_directories(qmlModule PUBLIC .)

qt_add_qml_module(qmlModule
    URI LR1Explorer
    VERSION 1.0
    SOURCES
        meshgeometry.hpp meshgeometry.cpp
    IMPORTS
        QtQuick3D
)