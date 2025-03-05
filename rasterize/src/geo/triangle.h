//
// Created by Boris N on 1/22/25.
//

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "obj3.h"
#include "../color.h"

class triangle {
public:
    std::array<Point3d, 3> vertices;
    std::array<color, 3> colors;

    triangle(): colors{ black(), black(), black() } {};

    triangle(const std::array<Point3d, 3>& v, const color& c):
        vertices { v },
        colors { c, c, c }
    {}

    triangle(Point3d one, Point3d two, Point3d tres, std::array<color, 3> c):
        vertices { std::move(one), std::move(two), std::move(tres) },
        colors { c }
    {}

    triangle operator*(const Point3d & scale) const {
        return {
            Point3d(vertices[0][0] * scale[0], vertices[0][1] * scale[1], vertices[0][2] * scale[2]),
            Point3d(vertices[1][0] * scale[0], vertices[1][1] * scale[1], vertices[1][2] * scale[2]),
            Point3d(vertices[2][0] * scale[0], vertices[2][1] * scale[1], vertices[2][2] * scale[2]),
            colors
        };
    }

    triangle operator+(const Point3d & matrix) const {
        return {
            vertices[0] + matrix,
            vertices[1] + matrix,
            vertices[2] + matrix,
            colors
        };
    }
};

#endif //TRIANGLE_H
