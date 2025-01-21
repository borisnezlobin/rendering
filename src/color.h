//
// Created by Boris N on 1/14/25.
//

#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

using color = vec3;

static color white() {
    return { 1, 1, 1 };
}

static color red() {
    return { 1, 0, 0 };
}

static color light_blue() {
    return { 0.5, 0.7, 1.0 };
}

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    static const interval intensity(0, 0.9999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif //COLOR_H
