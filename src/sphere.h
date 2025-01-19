//
// Created by Boris N on 1/16/25.
//

#ifndef SPHERE_H
#define SPHERE_H
#include "obj3.h"

class sphere: public obj3 {
public:
    sphere(const point3 &center, double radius): center { center }, radius { fmax(0, radius) } {}

    bool hit(ray &ray, double tmin, double tmax, hit_record &rec) const override {
        auto a = ray.direction().length_squared();
        auto h = dot(ray.direction(), (center - ray.origin()));
        auto c = (center - ray.origin()).length_squared() - radius * radius;

        const double disc = h * h - 4 * a * c;
        if (disc < 0) {
            return false;
        }

        const auto disc_root = sqrt(disc);
        auto root = (h - disc_root) / a;

        if (root <= tmin || tmax <= root) {
            root = (h + disc_root) / a;
            if (root <= tmin || tmax <= root)
                return false;
        }

        rec.t = root;
        rec.point = ray.at(rec.t);

        vec3 outward_normal = (rec.point - center) / radius;
        rec.set_face_normal(ray, outward_normal);

        return true;
    }

private:
    point3 center;
    double radius;
};

#endif //SPHERE_H
