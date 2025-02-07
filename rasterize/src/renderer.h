//
// Created by Boris N on 1/27/25.
//

#include "camera.h"
#include "geo/obj3.h"
#include "bitmap.h"
#include "util/rasterize.h"

#ifndef RENDERER_H
#define RENDERER_H

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
    void render_line(Point3d start, Point3d end);
    void draw_line(coord start, coord end, double thickness);
    void render_point(Point3d point, color c) {
        coord screen = cam.plane_coord_to_screen(point_to_plane(cam, point));
        std::clog << screen << "\n";
        b.set_pixel(screen, c);
    }

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
