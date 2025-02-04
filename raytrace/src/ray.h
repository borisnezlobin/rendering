//
// Created by Boris N on 1/15/25.
//

#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
    ray() {}
    ray(const point3& orig, const vec3& dir) : orig(orig), dir(dir) {}

    const point3& origin() const  { return orig; }
    const vec3& direction() const { return dir; }

    point3 at(double t) const {
        return origin() + direction() * t;
    }

private:
    point3 orig;
    vec3 dir;
};

#endif //RAY_H
