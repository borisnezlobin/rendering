cmake_minimum_required(VERSION 3.30)
project(rendering)

include_directories(/usr/local/include)

set(CMAKE_CXX_STANDARD 26)

add_executable(rendering src/main.cpp
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
)
