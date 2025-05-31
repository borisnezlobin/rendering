//
// Created by Boris N on 5/30/25.
//


#include <SDL.h>
#include <vector>
#include <iostream>

#include "camera.h"
#include "renderer.h"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

constexpr int IMAGE_WIDTH = 1024;
constexpr int IMAGE_HEIGHT = 512;

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize, quitting\n";
        return 1;
    }

    camera cam(1, IMAGE_WIDTH, IMAGE_HEIGHT);
    renderer r(IMAGE_WIDTH, 2, 1, cam);
    long long start = now();
    cam.set_position(Point3d(0, 5, 0));

    cam.set_rotation(Quaterniond(0.9914, 0.13052, 0, 0));

    SDL_Window* window = SDL_CreateWindow("Bitmap Viewer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,
        WINDOW_WIDTH, WINDOW_HEIGHT);

    obj3 cube = obj3::load_model("cube.obj");

    for (double x = -10.1; x < 11; x += 3) {
        for (double z = 15.1; z < 30; z += 3) {
            r.render_obj(
                cube,
                Point3d(x, 0, z),
                Quaterniond(1, 0, 0, 0),
                Point3d(
                    random_double(0.15, 0.5),
                    random_double(0.15, 0.25),
                    random_double(0.15, 0.5)
                )
            );
        }
    }

    std::vector<uint32_t> bitmap = r.export_bitmap();

    SDL_Texture* imgTex = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,
        IMAGE_WIDTH, IMAGE_HEIGHT);
    SDL_UpdateTexture(imgTex, nullptr, bitmap.data(), IMAGE_WIDTH * sizeof(uint32_t));

    bool running = true;
    int offsetX = 0, offsetY = 0;
    bool dragging = false;
    int lastX = 0, lastY = 0;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                dragging = true;
                lastX = e.button.x;
                lastY = e.button.y;
            } else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
                dragging = false;
            } else if (e.type == SDL_MOUSEMOTION && dragging) {
                offsetX += e.motion.xrel;
                offsetY += e.motion.yrel;
            }
        }

        SDL_RenderClear(renderer);

        SDL_Rect srcRect = { -offsetX, -offsetY, WINDOW_WIDTH, WINDOW_HEIGHT };
        SDL_Rect dstRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
        SDL_RenderCopy(renderer, imgTex, &srcRect, &dstRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(imgTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}