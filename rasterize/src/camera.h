//
// Created by Boris N on 1/22/25.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "geo/coord.h"
#include "util/util.h"

class camera {
public:
    explicit camera(double f, int image_width, int image_height):
        focal_depth(f),
        image_height(image_height),
        image_width(image_width),
        pos(0, 0, 0)
    {}

    Point3d center() const { return pos; }
    Quaterniond dir() const { return direction; }
    double f() const { return focal_depth; }

    void set_position(const Point3d &point) {
        pos = point;
    }

    void set_rotation(const Quaterniond &quat) {
        direction = quat;
    }

    coord plane_coord_to_screen(const Point2d &pos) const {
        return {
            static_cast<int>(-1 * pos.x() * pixels_per_world_unit),
            static_cast<int>(pos.y() * pixels_per_world_unit)
        };
    }

    bool coord_on_screen(const coord& c) const {
        auto x_limit = (image_width / 2);
        auto y_limit = (image_height / 2);

        return fabs(c.x()) < x_limit && fabs(c.y()) < y_limit;
    }

private:
    Point3d pos;
    Quaterniond direction;
    double focal_depth;
    int image_width;
    int image_height;
    int pixels_per_world_unit = static_cast<int>(1000.0 / focal_depth);
};

#endif //CAMERA_H
