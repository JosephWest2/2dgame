cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(2dshooter VERSION 1.0)

add_subdirectory(external/sdl)
add_subdirectory(external/sdl_image)

add_executable(2dshooter main.cpp)
target_include_directories(2dshooter
    PRIVATE external/sdl/include
    PRIVATE external/sdl_image/include
)
target_link_libraries(2dshooter
    PRIVATE SDL3::SDL3
    PRIVATE SDL3_image::SDL3_image
)
