//
// Created by Boris N on 1/19/25.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "material.h"
#include "obj3.h"
#include "util.h"

class camera {
public:
    int image_width;
    double aspect_ratio = 19.0 / 6;
    const int aa_samples = 31;
    const int max_depth = 50;

    camera(int width, double ar) {
        this->image_width = width;
        this->aspect_ratio = ar;
        this->image_height = int(this->image_width / aspect_ratio);
    }

    void render(const obj3& world) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            for (int i = 0; i < image_width; i++) {
                std::clog << "\rScanlines remaining: " << image_height - j << ' ' << std::flush;

                color pixel_color;
                int samples_used = 0;
                int num_samples = aa_samples * aa_samples;
                for (int sample = 0; sample < num_samples; sample++) {
                    // anti-aliasing
                    // todo: switch to FXAA
                    if (
                        random_double() > 0.35 ||
                        (sample == num_samples - 1 && samples_used == 0) &&
                        samples_used < num_samples * 0.75
                    ) {
                        pixel_color += ray_color(get_ray(i, j, sample), max_depth, world);
                        samples_used++;
                    }

                    // pixel_color += ray_color(get_ray(i, j, sample), world);
                }

                write_color(std::cout, pixel_color / (samples_used));
            }
        }

        std::clog << "\nDone.\n";
    }

private:
    point3 center;
    point3 pixel00_loc;
    int image_height;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;

    void initialize() {
        center = point3(0, 0, 0);

        constexpr auto focal_length = 1.0;
        constexpr auto viewport_height = 2.0;
        const auto viewport_width = viewport_height * (double(image_width)/image_height);
        const auto viewport_u = vec3(viewport_width, 0, 0);
        const auto viewport_v = vec3(0, -viewport_height, 0);

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        auto upper_left = center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
        pixel00_loc = upper_left + 0.5 * pixel_delta_u + 0.5 * pixel_delta_v;
    }

    vec3 sample_square(int sample) const {
        auto x = (sample % aa_samples) * (1.0 / aa_samples) - 0.5;
        auto y = (sample / aa_samples) * (1.0 / aa_samples) - 0.5;
        return { x, y, 0 };
        // return { random_double() - 0.5, random_double() - 0.5, 0 };
    }

    static color ray_color(ray r, int max_depth, const obj3 &world) {
        if (max_depth <= 0) {
            return { 0, 0, 0 };
        }
        hit_record record;

        if (world.hit(r, 0.001, infinity, record)) {
            ray dir;
            color attenuation;
            if (record.mat->scatter(r, record, attenuation, dir))
                return attenuation * ray_color(dir, max_depth - 1, world);
            return { 0, 0, 0 };
        }

        double a = 0.5 * (unit_vector(r.direction()).y() + 1);
        return a * light_blue() + (1 - a) * white();
    }

    ray get_ray(const int i, const int j, int sample) {
        // gets a ray from the camera and randomly sampled in the pixel (i, j)
        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        auto offset = sample_square(sample);
        pixel_center += offset * pixel_delta_u + offset * pixel_delta_u;

        return { center, unit_vector(pixel_center - center) };
    }
};

#endif //CAMERA_H
