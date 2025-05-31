cmake_minimum_required(VERSION 3.15)
project(rendering)

set(CMAKE_CXX_STANDARD 20)

set(SDL2_INCLUDE_DIR "/opt/homebrew/opt/sdl2/include/SDL2")
set(SDL2_LIBRARY "/opt/homebrew/opt/sdl2/lib/libSDL2.dylib")

include_directories(${SDL2_INCLUDE_DIR})
include_directories(/usr/local/include)

add_executable(rendering
        src/window.cpp
        src/bitmap.h
        src/color.h
        src/geo/triangle.h
        src/geo/vertex.h
        src/util/rasterize.h
        src/geo/coord.h
        src/camera.h
        src/geo/obj3.h
        src/renderer.cpp
        src/renderer.h
        src/geo/obj3.cpp
        src/util/tiny_obj_loader.cpp
)

target_link_libraries(rendering ${SDL2_LIBRARY})