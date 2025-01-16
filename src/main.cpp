//
// Created by Boris N on 1/14/25.
//

#include "main.h"

#include <iostream>

#include "color.h"
#include "ray.h"

double hit_sphere(point3 center, double radius, ray ray) {
    auto a = ray.direction().length_squared();
    auto h = dot(ray.direction(), (center - ray.origin()));
    auto c = (center - ray.origin()).length_squared() - radius * radius;

    const double disc = h * h - 4 * a * c;
    if (disc < 0) {
        return -1.0;
    } else {
        return (-b - std::sqrt(disc)) / (2 * a);
    }
}

color ray_color(const ray& r) {
    point3 center(0, 0, -1);
    double hit = hit_sphere(center, 0.5, r);
    if (hit != -1.0) {
        vec3 norm = unit_vector(r.at(hit) - center);

        return 0.5 * (vec3(norm.x(), norm.y(), norm.z()) + vec3(1, 1, 1));
    }

    double a = 0.5 * (r.direction().y() + 1);
    return a * light_blue() + (1 - a) * white();
}

int main() {
    // image params
    int image_width = 1080;
    int image_height = 607;

    // camera params
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width)/image_height);
    point3 camera_center = point3(0, 0, 0);

    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - (viewport_u / 2) - (viewport_v / 2);
    auto pixel00_loc = viewport_upper_left + (0.5 * (pixel_delta_u + pixel_delta_v));

    // render stuff
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            std::clog << "\rScanlines remaining: " << image_height - j << ' ' << std::flush;

            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, unit_vector(ray_direction));

            write_color(std::cout, ray_color(r));
        }
    }

    std::clog << "\nDone.\n";
}
