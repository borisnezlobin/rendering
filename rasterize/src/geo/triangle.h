//
// Created by Boris N on 1/22/25.
//

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../color.h"
#include <array>

class triangle {
public:
    std::array<Point3d, 3> vertices;

    triangle(const std::array<Point3d, 3>& v, const color& c):
        vertices { v }
    {}

    triangle(Point3d one, Point3d two, Point3d three):
        vertices { std::move(one), std::move(two), std::move(three) }
    {}
};

#endif //TRIANGLE_H
