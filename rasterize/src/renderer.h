//
// Created by Boris N on 1/27/25.
//


#ifndef RENDERER_H
#define RENDERER_H

#include "geo/obj3.h"
#include "bitmap.h"

class renderer {
public:
    renderer(int width, double ar, double f, camera &cam) :
        width { width },
        aspect_ratio { ar },
        focal_depth { f },
        height(width / aspect_ratio),
        cam(cam)
    {}

    void render_obj(obj3 &obj);
    void render_line(Point3d start, Point3d end, color c);
    void draw_line(coord start, coord end, double thickness, color c);
    void render_point(Point3d point, color c) {
        coord screen = cam.plane_coord_to_screen(point_to_plane(cam, point));
        std::clog << screen << "\n";
        b.set_pixel(screen, c);
    }

    void render_triangle(const triangle& tri);

    Point3d barycentric(Point2d vertices[], Point2d point);

    void write() {
        b.write();
    }

private:
    camera& cam;
    int width;
    double aspect_ratio;
    int height;
    double focal_depth;
    bitmap b { width, height };
};

#endif //RENDERER_H
