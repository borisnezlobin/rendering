//
// Created by Boris N on 1/22/25.
//

#ifndef RASTERIZE_H
#define RASTERIZE_H

#include "../camera.h"

using Eigen::Vector3d;

inline Vector3d relative_to(const Vector3d& original, const Quaterniond& rot) {
    return rot.inverse().toRotationMatrix() * original;
}

inline Point3d point_relative_to_camera(const camera& cam, const Point3d& point) {
    return relative_to(point - cam.center(), cam.dir());
}

/**
 * returns the 2D (z=focal_depth) point projection of `point`
 *
 * @param point the point, *relative to the camera's position and orientation*, to project
 */
inline Point2d point_to_plane(const camera& cam, const Point3d &point) {
    Point3d transform = point;

    // perspective project the transform (now that the camera is at (0,0,0) and aligned with z-axis)
    if (transform.z() == 0)
        transform.z() = cam.f();

    const auto scale = cam.f() / transform.z();
    return {
        (transform.x() * scale),
        (transform.y() * scale) * (transform.z() < 0 ? -1 : 1)
    };
}



#endif //RASTERIZE_H
