//
// Created by Boris N on 1/14/25.
//


#include "./util/util.h"
#include "camera.h"
#include "renderer.h"
#include "Eigen/Dense"

int main() {
    int image_width = 800;
    double aspect_ratio = 2;
    camera cam(1, image_width, image_width / aspect_ratio);
    renderer r(800, 2, 1, cam);
    cam.set_position(Point3d(0, 5, 0));
    cam.set_rotation(Quaterniond(1, 0, 0, 0));
    // cam.set_rotation(Quaterniond(0.9914, 0.13052, 0, 0)); // looking down 15 degrees or something
    for (int i = -10; i < 10; i++) {
        r.render_line(Point3d(i, 0, -1), Point3d(i, 0, 10));
        // for (int j = -10; j < 0; j++) {
        //     r.render_line(Point3d(-10, 0, j), Point3d(10, 0, j));
        // }
    }

    r.write();
}