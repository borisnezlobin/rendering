//
// Created by Boris N on 1/14/25.
//

#include "main.h"

#include <iostream>

#include "color.h"

int main() {

    // Image

    int image_width = 256;
    int image_height = 256;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            std::clog << "\rScanlines remaining: " << image_height - j << ' ' << std::flush;
            color pixel_color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0.0);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\nDone.\n";
}
