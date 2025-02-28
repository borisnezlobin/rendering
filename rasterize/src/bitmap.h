//
// Created by Boris N on 1/22/25.
//

#ifndef BITMAP_H
#define BITMAP_H
#include <utility>

#include "color.h"
#include "geo/coord.h"
#include "geo/triangle.h"

class bitmap {
public:
    bitmap(int width, int height) : width(width), height(height), map(width * height, black()), depth_map(width * height, infinity) {};
    void set_pixel(const int x, const int y, const color col) {
        if (!pixel_on_screen(x, y)) return;
        int index = x + y * width;
        if (index < 0) return;
        if (index > map.size()) return;

        map.at(index) = col;
    }

    void set_pixel(coord c, const color col) {
        if (!pixel_on_screen(c.x(), c.y())) return; // throw err or something idk
        int index = -c.x() - c.y() * width + (width / 2) + (height / 2) * width;
        if (index >= map.size() || index < 0) return; // throw err
        map.at(index) = col;
    }

    void set_pixel_if_deep(coord c, const color col, double depth) {
        if (!pixel_on_screen(c.x(), c.y())) return; // throw err or something idk
        int index = -c.x() - c.y() * width + (width / 2) + (height / 2) * width;
        if (index >= map.size() || index < 0) return; // throw err

        if (depth > depth_map.at(index)) return;

        map.at(index) = col;
        depth_map.at(index) = depth;
    }

    void add_triangle(triangle tri) {

    }

    void write() {
        std::cout << "P3\n" << width << ' ' << height << "\n255\n";

        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                std::clog << "\rScanlines remaining: " << height - j << ' ' << std::flush;
                write_color(std::cout, get_pixel(j, i));
            }
        }

        std::clog << "\nDone.\n";
    }

    bool pixel_on_screen(const int x, const int y) {
        return !(fabs(x) > width / 2 || fabs(y) > height / 2);
    }

    color get_pixel(int x, int y) {
        return map.at(x * width + y);
    }

    color get_pixel(coord c) {
        return map.at(c.x() * width + c.y());
    }

private:
    int width;
    int height;
    std::vector<color> map;
    std::vector<double> depth_map;
};

#endif //BITMAP_H
