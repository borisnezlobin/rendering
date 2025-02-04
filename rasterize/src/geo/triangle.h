//
// Created by Boris N on 1/22/25.
//

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../color.h"

class triangle {
public:
    triangle(Point3d vertices[], const color &color):
        vertices { vertices[0], vertices[1], vertices[2] }
        // colors { color, color, color }
    {}
    Point3d vertices[];
    // color colors[];
};

#endif //TRIANGLE_H
