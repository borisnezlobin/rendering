//
// Created by Boris N on 1/18/25.
//

#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
using std::shared_ptr;
using std::make_shared;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.141592653589793; // NASA uses 15 digits to land on the moon

inline double to_radians(double degrees) {
    return degrees * pi / 180.0;
}

#include "color.h"
#include "ray.h"
#include "vec3.h"

#endif //UTIL_H
