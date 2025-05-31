//
// Created by Boris N on 1/14/25.
//

#include "renderer.h"

void draw_grid(renderer &r) {
    for (int i = -10; i < 11; i++) {
        r.render_line(Point3d(i, 0, -1), Point3d(i, 0, 30), dark_grey());
        for (int j = 0; j < 31; j++) {
            r.render_line(Point3d(-10, 0, j), Point3d(10, 0, j), dark_grey());
        }
    }
}

int main() {
    int image_width = 1600;
    double aspect_ratio = 2;
    camera cam(1, image_width, image_width / aspect_ratio);
    renderer r(image_width, 2, 1, cam);
    long long start = now();
    cam.set_position(Point3d(0, 5, 0));

    cam.set_rotation(Quaterniond(0.9914, 0.13052, 0, 0)); // looking down 15 degrees or something

    triangle t1(
        Point3d(0, 0, 20),
        Point3d(-1, 0, 20),
        Point3d(-1, 1, 24),
        { red(), red(), blue() }
    );

    triangle t2(
        Point3d(-2, 0, 24),
        Point3d(-1, 0, 20),
        Point3d(-1, 1, 24),
        { white(), white(), green() }
    );

    triangle t3(
        Point3d(-2, 0, 24),
        Point3d(-1, 0, 20),
        Point3d(-2, 0, 20),
        { light_blue(), light_blue(), white() }
    );

    // draw some lines in a grid
    draw_grid(r);

    obj3 cube = obj3::load_model("cube.obj");

    // r.render_obj(
    //     cube,
    //     Point3d(10, 0, 20),
    //     Quaterniond(1, 0, 0, 0),
    //     Point3d(
    //         random_double(1, 1),
    //         random_double(1, 1),
    //         random_double(1, 1)
    //     )
    // );

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

    std::clog << "render time: " << now() - start << " ns" << std::endl;
    std::clog << "expected fps: " << 1e9 / (now() - start) << std::endl;

    // write
    r.write();
}