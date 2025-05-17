cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(2dshooter VERSION 1.0)

add_executable(2dshooter main.cpp)
target_include_libraries(2dshooter
    PRIVATE external/sdl
)
