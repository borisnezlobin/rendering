//
// Created by Boris N on 1/14/25.
//

#ifndef VEC3_H
#define VEC3_H

#include "util.h"

class vec3 {
public:
    double e[3];

    vec3() : e{0, 0, 0} {}
    vec3(double e1, double e2, double e3): e{e1, e2, e3} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(double t) {
        return *this *= 1/t;
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    static vec3 random() {
        return { random_double(), random_double(), random_double() };
    }

    static vec3 random(double min, double max) {
        return { random_double(min, max), random_double(min, max), random_double(min, max) };
    }

    bool near_zero() const {
        auto s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }
};

using point3 = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return { u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2] };
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return { u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2] };
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return { u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2] };
}

inline vec3 operator*(double t, const vec3& v) {
    return { t * v.e[0], t * v.e[1], t * v.e[2] };
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

inline vec3 reflect(const vec3 &v, const vec3 &n) {
    return v - 2 * dot(v, n) * n;
}

inline vec3 random_unit_vector() {
    vec3 vec = vec3::random(-0.5, 0.5);
    while (vec.length_squared() < 1e-160) {
        vec = vec3::random(-0.5, 0.5);
    }
    return unit_vector(vec);
}

inline vec3 diffuse_direction(vec3 normal) {
    vec3 ret = random_unit_vector();
    if (dot(ret, normal) < 0) {
        ret = -ret;
    }
    return ret;
}

#endif //VEC3_H
