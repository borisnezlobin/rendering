//
// Created by Boris N on 1/14/25.
//

#ifndef COLOR_H
#define COLOR_H

#include "./util/interval.h"
#include "./util/util.h"


class color {
public:
    color(double x, double y, double z): e{x, y, z} {}
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

private:
    std::array<double, 3> e;
};

static color white() {
    return { 1, 1, 1 };
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

#endif //COLOR_H
