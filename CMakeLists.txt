cmake_minimum_required(VERSION 3.5)

project(komodo-gui-launcher)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

add_executable(komodo-gui-launcher
    main.cpp
    MainWindow.h
    MainWindow.cpp
)

target_link_libraries(komodo-gui-launcher
    Qt5::Widgets
    Qt5::Network
)
