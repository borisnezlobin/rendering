//
// Created by Boris N on 1/16/25.
//

#ifndef SPHERE_H
#define SPHERE_H
#include "obj3.h"

class sphere: public obj3 {
public:
    sphere(const point3 &center, double radius): center { center }, radius { fmax(0, radius) } {}

    bool hit(ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
        vec3 oc = center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius*radius;

        auto discriminant = h*h - a*c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrtd) / a;
        if (root <= ray_tmin || ray_tmax <= root) {
            root = (h + sqrtd) / a;
            if (root <= ray_tmin || ray_tmax <= root)
                return false;
        }

        rec.t = root;
        rec.point = r.at(rec.t);
        rec.normal = (rec.point - center) / radius;

        return true;
    }

private:
    point3 center;
    double radius;
};

#endif //SPHERE_H
