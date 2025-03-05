//
// Created by Boris N on 3/5/25.
//

#ifndef MATHUTIL_H
#define MATHUTIL_H

#include <cmath>
#include <random>

inline bool fuzzy_compare(double a, double b) {
    return std::abs(a - b) < 0.0001;
}

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

#endif //MATHUTIL_H
