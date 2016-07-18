cmake_minimum_required(VERSION 2.6)
project(uim-led)

set(CMAKE_C_FLAGS "-Wall -Wextra -pthread")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src)
