//
// Created by Boris N on 1/22/25.
//

#ifndef BITMAP_H
#define BITMAP_H

#include "geo/coord.h"

class bitmap {
public:
    bitmap(int width, int height) :
        width(width),
        height(height),
        map(width * height, black()),
        depth_map(width * height, infinity),
        dot_product_map(width * height, infinity)
    {};

    // USE WITH CAUTION. definitely correct
    void set_pixel(coord c, const color col) {
        // check if pixel is on screen
        if (!pixel_on_screen(c.x(), c.y())) return;
        // calculate index of pixel given that coordinates (for example, 0,0 is at the center of the screen)
        // if width = 480 and height = 240, then coordinates range from (-240, -120) to (239, 119)
        // index should be 0 at (-240, -120)
        int index = index_of(c.x(), c.y());
        if (index >= map.size() || index < 0) return;
        map.at(index) = col;
    }

    void set_pixel_if_deep(
        coord c,
        const color col,
        double depth,
        double dot_product = infinity
    ) {
        if (!pixel_on_screen(c.x(), c.y())) return;
        int index = index_of(c.x(), c.y());
        if (index >= map.size() || index < 0) return;
        if (depth > depth_map.at(index) + 0.001) return;
        // if this pixel is set and the depths are the same, compare dot products
        if (fuzzy_compare(depth, depth_map.at(index))) {
            if (dot_product <= dot_product_map.at(index)) return;
        }

        map.at(index) = col;
        depth_map.at(index) = depth;
        dot_product_map.at(index) = dot_product;
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
        // check whether given coordinate is on screen
        return x >= -width/2 && x < width/2 && y >= -height/2 && y < height/2;
    }

    double depth(int x, int y) {
        if (!pixel_on_screen(x, y)) return -1;
        return depth_map.at(index_of(x, y));
    }

    color get_pixel(int x, int y) {
        return map.at(x * width + y);
    }

    color get_pixel(coord c) {
        return map.at(c.x() * width + c.y());
    }

    std::vector<uint32_t> export_bitmap() {
        std::vector<uint32_t> result(width * height);
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                color c = get_pixel(j, i);
                result[j * width + i] = c.to_uint32();
            }
        }

        return result;
    }

private:
    int width;
    int height;
    std::vector<color> map;
    std::vector<double> depth_map;
    std::vector<double> dot_product_map;

    int index_of(int x, int y) {
        return (x + width/2) + (y + height/2) * width;;
    }
};

#endif //BITMAP_H
