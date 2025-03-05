//
// Created by Boris N on 1/18/25.
//

#ifndef UTIL_H
#define UTIL_H

#include <utility>
#include <array>
#include <iostream>
#include <limits>
#include <memory>
#include <fstream>

#include "Eigen/Dense"
#include "Eigen/Geometry"

using std::shared_ptr;
using std::make_shared;

using namespace Eigen;

using Point3d = Vector3d;
using Point2d = Vector2d;
using AABB = AlignedBox2d;
using AABB3d = AlignedBox3d;

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


#include "tiny_obj_loader.h"
#include "./rasterize.h"

#endif //UTIL_H
