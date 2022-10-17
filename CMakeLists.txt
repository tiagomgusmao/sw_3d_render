cmake_minimum_required(VERSION 3.7)

project(SW_3D_Render)

find_package(SDL2 REQUIRED)

# SDL2 image doesn't come with a cmake module
# Solution came from https://trenki2.github.io/blog/2017/07/04/using-sdl2-image-with-cmake/
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

add_executable(sw_3d_render main.cpp)
target_link_libraries(sw_3d_render ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})