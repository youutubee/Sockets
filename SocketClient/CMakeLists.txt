cmake_minimum_required(VERSION 3.29)
project(SocketClient C)

set(CMAKE_C_STANDARD 11)


find_path(TheHeaderFile socketUtil.h
        PATHS /Users/shrijankatiyar/CLionProjects/SocketUtil)

find_library(TheLibrary
        NAMES SocketUtil
        PATHS /Users/shrijankatiyar/CLionProjects/SocketUtil/cmake-build-debug)

add_executable(SocketClient main.c
        main.c)

include_directories(${TheHeaderFile})
target_link_libraries(${PROJECT_NAME} ${TheLibrary})
