cmake_minimum_required(VERSION 3.16)
project(ShootingGame VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(ShootingGame
    main.cpp
    gamewindow.cpp
    gamewindow.h
)

target_link_libraries(ShootingGame PRIVATE Qt6::Widgets)
