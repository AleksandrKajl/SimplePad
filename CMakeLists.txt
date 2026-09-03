cmake_minimum_required(VERSION 3.16)

project(SimplePad VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)

add_executable(SimplePad WIN32
    SimplePad/FileSys.cpp
    SimplePad/FileSys.h
    SimplePad/SimplePad.cpp
    SimplePad/SimplePad.h
    SimplePad/SimplePad.qrc
    SimplePad/SimplePad.ui
    SimplePad/main.cpp
)

target_link_libraries(SimplePad PRIVATE
    Qt${QT_VERSION_MAJOR}::PrintSupport
    Qt${QT_VERSION_MAJOR}::Widgets
)
