//
// Created by Boris N on 1/16/25.
//

#ifndef OBJ3_H
#define OBJ3_H

#include "util.h"

class material;

class hit_record {
public:
    point3 point;
    vec3 normal;
    double t{};
    shared_ptr<material> mat;
    bool front_face{};

    void set_face_normal(ray r, vec3 outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class obj3 {
public:
    virtual ~obj3() = default;
    virtual bool hit(ray& r, double tmin, double tmax, hit_record& rec) const = 0;
};

#endif //OBJ3_H
