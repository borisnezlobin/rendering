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
    std::array<Point2d, 3> uvs;
    unsigned char * tex = nullptr;
    int tex_width = 0;

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
        triangle result(*this);
        for (auto & vertex : result.vertices) {
            vertex = Point3d(vertex.x() * scale.x(), vertex.y() * scale.y(), vertex.z() * scale.z());
        }
        return result;
    }

    triangle operator+(const Point3d & matrix) const {
        triangle result(*this);
        for (auto & vertex : result.vertices) {
            vertex = vertex + matrix;
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const triangle& tri) {
        os << "Triangle: [("
           << tri.vertices[0].x() << ", " << tri.vertices[0].y() << ", " << tri.vertices[0].z() << "), ("
        << tri.vertices[1].x() << ", " << tri.vertices[1].y() << ", " << tri.vertices[1].z() << "), ("
        << tri.vertices[2].x() << ", " << tri.vertices[2].y() << ", " << tri.vertices[2].z() << ")]";
        return os;
    }
};

#endif //TRIANGLE_H
