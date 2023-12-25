cmake_minimum_required(VERSION 3.22)

project(Lab6Socket)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF)

add_subdirectory(client)
add_subdirectory(server)