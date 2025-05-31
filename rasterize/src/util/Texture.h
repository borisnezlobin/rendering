//
// Created by Boris N on 5/31/25.
//

#ifndef TEXTURE_H
#define TEXTURE_H
#include "../color.h"


class Texture {
public:
    int width, height;
    std::vector<color> pixels;

    Texture(const std::string& filename); // use stb_image or something to fill pixels

    color sample(float u, float v) {
        int x = clamp(int(u * width), 0, width - 1);
        int y = clamp(int((1 - v) * height), 0, height - 1); // y flipped
        return pixels[y * width + x];
    }

private:
    int clamp(int value, int min, int max) {
        return std::max(min, std::min(value, max));
    }
};



#endif //TEXTURE_H
