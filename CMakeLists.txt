cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(2dgame VERSION 1.0)

add_subdirectory(external/sdl)
add_subdirectory(external/sdl_image)
add_subdirectory(external/toml11)

add_executable(2dgame src/main.cpp)
target_include_directories(2dgame
    PRIVATE external/sdl/include
    PRIVATE external/sdl_image/include
    PRIVATE toml11
)
target_link_libraries(2dgame
    PRIVATE SDL3::SDL3
    PRIVATE SDL3_image::SDL3_image
    PRIVATE toml11
)
