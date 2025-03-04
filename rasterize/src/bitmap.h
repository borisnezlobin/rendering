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
    bitmap(int width, int height) :
        width(width),
        height(height),
        map(width * height, black()),
        depth_map(width * height, infinity)
    {};

    // USE WITH CAUTION
    void set_pixel(coord c, const color col) {
        // if (!pixel_on_screen(c.x(), c.y())) return; // throw err or something idk
        // calculate index of pixel given that coordinates (for example, 0,0 is at the center of the screen)
        // if width = 480 and height = 240, then coordinates range from (-240, -120) to (239, 119)
        // index should be 0 at (-240, -120)
        int index = (c.x() + width/2) + (c.y() + height/2) * width;
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

    void write() {
        std::cout << "P3\n" << width << ' ' << height << "\n255\n";

        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                std::clog << "\rScanlines remaining: " << height - j << ' ' << std::flush;
                color c = get_pixel(j, i);
                write_color(std::cout, c);
            }
        }

        std::clog << "\nDone.\n";
    }

    bool pixel_on_screen(const int x, const int y) {
        return (x < (width) / 2.0 && y < (height + 1) / 2.0 && x > (-(width + 1) / 2.0) && y > (-(height) / 2.0));
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
