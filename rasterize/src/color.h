//
// Created by Boris N on 1/14/25.
//

#ifndef COLOR_H
#define COLOR_H

#include "./util/interval.h"
#include "./util/util.h"
#include "./util/mathutil.h"

class color {
public:
    color(double x, double y, double z): e{x, y, z} {}
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    color operator+=(const color value) const {
        return { e[0] + value.e[0], e[1] + value.e[1], e[2] + value.e[2] };
    }

    color operator/(int i) const {
        return { e[0] / i, e[1] / i, e[2] / i };
    }

    color operator/(double i) const {
        return { e[0] / i, e[1] / i, e[2] / i };
    }

    bool operator==(const color & color) const {
        return fuzzy_compare(e[0], color.e[0]) && fuzzy_compare(e[1], color.e[1]) && fuzzy_compare(e[2], color.e[2]);
    }

    color operator*(double x) const {
        return { e[0] * x, e[1] * x, e[2] * x };
    }

    color operator+(const color & color) const {
        return { e[0] + color.e[0], e[1] + color.e[1], e[2] + color.e[2] };
    }

    static color random() {
        return { random_double(), random_double(), random_double() };
    }

private:
    std::array<double, 3> e;
};

static color white() {
    return { 1, 1, 1 };
}

static color green() {
    return { 0.2, 1, 0.2 };
}

static color dark_grey() {
    return { 0.2, 0.2, 0.2 };
}

static color black() {
    return { 0, 0, 0 };
}

static color red() {
    return { 1, 0, 0 };
}

static color light_blue() {
    return { 0.5, 0.7, 1.0 };
}

static color blue() {
    return { 0.2, 0.2, 1 };
}

inline void write_color(std::ostream& out, const color& pixel_color) {
    const auto r = linear_to_gamma(pixel_color.x());
    const auto g = linear_to_gamma(pixel_color.y());
    const auto b = linear_to_gamma(pixel_color.z());

    static const interval intensity(0, 0.9999);
    const int rbyte = int(256 * intensity.clamp(r));
    const int gbyte = int(256 * intensity.clamp(g));
    const int bbyte = int(256 * intensity.clamp(b));

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

inline color mix_colors(Point3d weights, std::array<color, 3> colors) {
    Point3d ret(0, 0, 0);
    for (int i = 0; i < 3; i++) {
        Point3d c(colors.at(i).x(), colors.at(i).y(), colors.at(i).z());
        ret = ret + (c * weights[i]);
    }

    ret /= 3;
    return { ret.x(), ret.y(), ret.z() };
}

#endif //COLOR_H
