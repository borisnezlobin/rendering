//
// Created by Boris N on 1/27/25.
//


#include "renderer.h"

#include "util/rasterize.h"

void renderer::render_obj(obj3 &obj) {
    for (auto &tri : obj.get_tris()) {
        for (int i = 0; i < 3; i++) {
            Point3d temp = tri.vertices[i];
            Point2d plane = point_to_plane(cam, temp);
            coord screen = cam.plane_coord_to_screen(plane);
            if (cam.coord_on_screen(screen)) {
                b.set_pixel(screen, light_blue());
            }
        }
    }
}

void renderer::render_line(Point3d start, Point3d end) {
    // coord s = cam.plane_coord_to_screen(point_to_plane(cam, start));
    // coord e = cam.plane_coord_to_screen(point_to_plane(cam, end));
    // draw_line(s, e, 1);
    Point3d s = point_relative_to_camera(cam, start);
    Point3d e = point_relative_to_camera(cam, end);

    if (s.z() < focal_depth && e.z() < focal_depth) {
        return;
    }

    Point3d v = e - s;
    if (v.z() == 0) {
        draw_line(
            cam.plane_coord_to_screen(point_to_plane(cam, s)),
            cam.plane_coord_to_screen(point_to_plane(cam, e)),
            1
        );
        return;
    }
    // solve for t with equation line_intercept.z/(focal - v.z) = t
    double t = (focal_depth - s.z()) / (v.z());
    Point3d plane_intercept = s + t * v;

    if (s.z() < focal_depth) {
        s = plane_intercept;
    } else if (e.z() < focal_depth) {
        e = plane_intercept;
    }

    draw_line(
        cam.plane_coord_to_screen(point_to_plane(cam, s)),
        cam.plane_coord_to_screen(point_to_plane(cam, e)),
    1);
}

void renderer::draw_line(coord start, coord end, double thickness) {
    long long start_time = now();
    // render a line of thickness between two points
    int x0 = start.x();
    int y0 = start.y();
    int x1 = end.x();
    int y1 = end.y();

    if (x1 < x0) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    if (x1 - x0 == 0) {
        if (y0 > y1) std::swap(y0, y1);

        for (int y = y0; y < y1; y++) {
            for (int i = -thickness / 2; i < thickness / 2; i++) {
                b.set_pixel(coord(x0, y + i), light_blue());
            }
        }
        return;
    }

    double slope = 1.0 * (y1 - y0) / (x1 - x0);
    double y_intercept = y0 - slope * x0;
    for (int x = x0; x < x1; x++) {
        int y = slope * x + y_intercept;
        int i = -thickness / 2;
        do {
            b.set_pixel(coord(x, y + i), light_blue());
            i++;
        } while (i < thickness / 2);
    }

    std::clog << "line from" << start << " to " << end << " took " << now() - start_time << "µs" << std::endl;
}
