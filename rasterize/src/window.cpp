//
// Created by Boris N on 5/30/25.
//


#include <SDL.h>
#include <vector>
#include <iostream>

#include "camera.h"
#include "renderer.h"

constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 512;

constexpr int IMAGE_WIDTH = 1024;
constexpr int IMAGE_HEIGHT = 512;

struct RenderInstance {
    Point3d position;
    Quaterniond rotation;
    Point3d scale;
};

void draw_grid(renderer &r) {
    for (int i = -10; i < 11; i++) {
        r.render_line(Point3d(i, 0, -1), Point3d(i, 0, 30), dark_grey());
        for (int j = 0; j < 31; j++) {
            r.render_line(Point3d(-10, 0, j), Point3d(10, 0, j), dark_grey());
        }
    }
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize, quitting\n";
        return 1;
    }

    camera cam(1, IMAGE_WIDTH, IMAGE_HEIGHT);
    renderer r(IMAGE_WIDTH, 2, 1, cam);
    long long start = now();
    cam.set_position(Point3d(0, 5, 0));

    Quaterniond orientation = Quaterniond(0.9914, 0.13052, 0, 0);  // your initial rotation
    cam.set_rotation(orientation);

    std::vector<RenderInstance> scene_objects;

    SDL_Window* window = SDL_CreateWindow("Bitmap Viewer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(rend,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,
        WINDOW_WIDTH, WINDOW_HEIGHT);

    obj3 cube = obj3::load_model("./objects/swirlycube/untitled.obj");
    obj3 plant = obj3::load_model("./objects/plant/indoor plant_02.obj");

    bool running = true;
    int offsetX = 0, offsetY = 0;
    bool dragging = false;
    int lastX = 0, lastY = 0;

    float sensitivity = -0.005f; // don't worry about it being negative

    // create the scene objects
    for (double x = -10.1; x < 11; x += 3) {
        for (double z = 15.1; z < 30; z += 3) {
            scene_objects.push_back({
                Point3d(x, 0, z),
                Quaterniond(1, 0, 0, 0),
                Point3d(
                    random_double(0.15, 0.5),
                    random_double(0.15, 0.25),
                    random_double(0.15, 0.5)
                )
            });
        }
    }

    Point3d cam_pos(0, 5, 0);
    double move_speed = 0.5;

    // Track which keys are held
    bool moving_forward = false;
    bool moving_back = false;
    bool moving_left = false;
    bool moving_right = false;
    bool moving_up = false;
    bool moving_down = false;

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
                int dx = e.motion.x - lastX;
                int dy = e.motion.y - lastY;
                lastX = e.motion.x;
                lastY = e.motion.y;

                // incremental rotations
                Eigen::AngleAxisd yaw(-dx * sensitivity, Eigen::Vector3d::UnitY());
                Eigen::Vector3d right = orientation * Eigen::Vector3d::UnitX();
                Eigen::AngleAxisd pitch(dy * sensitivity, right);

                // apply rotation (note order: yaw * pitch * orientation)
                orientation = yaw * pitch * orientation;
                orientation.normalize();

                cam.set_rotation(orientation);
            } else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
                bool pressed = e.type == SDL_KEYDOWN;

                switch (e.key.keysym.sym) {
                    case SDLK_w: moving_forward = pressed; break;
                    case SDLK_s: moving_back = pressed; break;
                    case SDLK_a: moving_left = pressed; break;
                    case SDLK_d: moving_right = pressed; break;
                    case SDLK_q: moving_down = pressed; break;
                    case SDLK_e: moving_up = pressed; break;
                }
            }

            Eigen::Vector3d forward = orientation * Eigen::Vector3d(0, 0, -1);
            Eigen::Vector3d right   = orientation * Eigen::Vector3d(1, 0,  0);
            Eigen::Vector3d up      = Eigen::Vector3d(0, 1, 0); // world up

            if (moving_forward) cam_pos -= move_speed * forward;
            if (moving_back)    cam_pos += move_speed * forward;
            if (moving_right)   cam_pos -= move_speed * right;
            if (moving_left)    cam_pos += move_speed * right;
            if (moving_up)      cam_pos += move_speed * up;
            if (moving_down)    cam_pos -= move_speed * up;

            cam.set_position(cam_pos);
        }

        r.clear();

        draw_grid(r);

        for (const auto& obj : scene_objects) {
            r.render_obj(cube, obj.position, obj.rotation, obj.scale);
        }

        // draw a plant
        r.render_obj(plant, Point3d(0, 0, 30), Quaterniond(1, 0, 0, 0), Point3d(1, 1, 1));


        std::vector<uint32_t> bitmap = r.export_bitmap();
        SDL_UpdateTexture(texture, nullptr, bitmap.data(), IMAGE_WIDTH * sizeof(uint32_t));

        SDL_RenderClear(rend);
        SDL_Rect srcRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
        SDL_Rect dstRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
        SDL_RenderCopy(rend, texture, &srcRect, &dstRect);
        SDL_RenderPresent(rend);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}