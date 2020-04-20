cmake_minimum_required(VERSION 3.16)
project(talasound_ui)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


set(CMAKE_CXX_STANDARD 14)


include_directories(.)

find_package(Qt5 COMPONENTS Widgets Core Network Multimedia MultimediaWidgets REQUIRED)

add_executable(${PROJECT_NAME}
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        networkpi.cpp
        networkpi.h
        playermodel.cpp
        playermodel.h
        playlistmusicmodel.cpp
        playlistmusicmodel.h
        talasoundres.qrc)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Network Qt5::Multimedia Qt5::MultimediaWidgets)
