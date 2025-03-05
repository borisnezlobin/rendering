//
// Created by Boris N on 1/22/25.
//

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../color.h"

class triangle {
public:
    std::array<Point3d, 3> vertices;
    std::array<color, 3> colors;

    triangle(const std::array<Point3d, 3>& v, const color& c):
        vertices { v },
        colors { c, c, c }
    {}

    triangle(Point3d one, Point3d two, Point3d tres, std::array<color, 3> c):
        vertices { std::move(one), std::move(two), std::move(tres) },
        colors { c }
    {}
};

#endif //TRIANGLE_H
