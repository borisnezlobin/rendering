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

#include "Eigen/Dense"

using std::shared_ptr;
using std::make_shared;

using namespace Eigen;

using Point3d = Vector3d;
using Point2d = Vector2d;

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

inline long long now() {
    const std::chrono::nanoseconds ns = std::chrono::system_clock::now().time_since_epoch();
    return ns.count();
}

inline long long now_ms() {
    return now() / 1000000;
}

#endif //UTIL_H
