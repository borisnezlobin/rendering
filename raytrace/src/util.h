//
// Created by Boris N on 1/18/25.
//

#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

using std::shared_ptr;
using std::make_shared;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.141592653589793; // NASA uses 15 digits to land on the moon

inline double to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    return random_double() * (max - min) + min;
}

inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return std::sqrt(linear_component);

    return 0;
}

#include "color.h"
#include "ray.h"
#include "vec3.h"

#endif //UTIL_H
