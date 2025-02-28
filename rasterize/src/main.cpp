//
// Created by Boris N on 1/14/25.
//


#include "./util/util.h"
#include "renderer.h"

int main() {
    int image_width = 800;
    double aspect_ratio = 2;
    camera cam(1, image_width, image_width / aspect_ratio);
    renderer r(image_width, 2, 1, cam);
    cam.set_position(Point3d(0, 5, 0));

    const triangle t(
        Point3d(-1, 0, 20),
        Point3d(0, 0, 22),
        Point3d(1, 0, 20),
        { green(), green(), green() }
    );

    const triangle t2(
        Point3d(-1, 0, 20),
        Point3d(0, 0, 22),
        Point3d(2, 0, 20),
        { red(), red(), red() }
    );

    cam.set_rotation(Quaterniond(0.9914, 0.13052, 0, 0)); // looking down 15 degrees or something

    // draw some lines in a grid
    for (int i = -10; i < 11; i++) {
        r.render_line(Point3d(i, 0, -1), Point3d(i, 0, 30));
        for (int j = 0; j < 31; j++) {
            r.render_line(Point3d(-10, 0, j), Point3d(10, 0, j));
        }
    }

    // draw the triangles to check our renderer
    r.render_triangle(t);
    r.render_triangle(t2);

    // write
    r.write();
}