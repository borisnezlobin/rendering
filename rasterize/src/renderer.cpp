//
// Created by Boris N on 1/27/25.
//


#include "renderer.h"

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

void renderer::render_line(Point3d start, Point3d end, color c) {
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
            2,
            c
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
        2,
        c
    );
}

void renderer::draw_line(coord start, coord end, double thickness, color c) {
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
                b.set_pixel(coord(x0, y + i), c);
            }
        }
        return;
    }

    double slope = 1.0 * (y1 - y0) / (x1 - x0);
    double y_intercept = y0 - slope * x0;

    if (fabs(slope) > 1) {
        for (int x = x0; x < x1; x++) {
            int y = slope * x + y_intercept;
            int y2 = slope * (x + 1) + y_intercept;
            if (y2 < y) std::swap(y, y2);

            for (int i = -thickness / 2; i < thickness / 2; i++) {
                for (int j = y; j < y2; j++) {
                    b.set_pixel(coord(x, j + i), c);
                }
            }
        }
    } else {
        for (int x = x0; x < x1; x++) {
            int y = slope * x + y_intercept;

            for (int i = -thickness / 2; i < thickness / 2; i++) {
                for (int j = y; j < y + thickness; j++) {
                    b.set_pixel(coord(x + i, j), c);
                }
            }
        }
    }
}

void renderer::render_triangle(const triangle &tri) {
    // transform triangle to camera space
    Point3d vertices[3];
    Point2d texcoords[3];
    for (int i = 0; i < 3; i++) {
        vertices[i] = point_relative_to_camera(cam, tri.vertices[i]);
        coord c = cam.plane_coord_to_screen(point_to_plane(cam, vertices[i]));
        texcoords[i] = Point2d(c.x(), c.y());
    }

    // determine whether triangle has parts on the screen (i.e. worth rendering or not) and is ahead of the camera
    bool on_screen = false;
    for (int i = 0; i < 3; i++) {
        Point2d plane = point_to_plane(cam, vertices[i]);
        coord screen = cam.plane_coord_to_screen(plane);
        if (vertices[i].z() > cam.f() && cam.coord_on_screen(screen)) {
            on_screen = true;
            break;
        }
    }

    if (!on_screen) return;

    // since the triangle is at least somewhat on screen, we can render it.
    // first, we need to determine the bounding box of the triangle
    AABB3d aabb(vertices[0], vertices[1]);
    aabb.extend(vertices[2]);

    // clip the bounding box to the screen
    coord minc = cam.plane_coord_to_screen(point_to_plane(cam, aabb.min()));
    coord maxc = cam.plane_coord_to_screen(point_to_plane(cam, aabb.max()));

    minc = coord(std::max(-width / 2, minc.x()), std::max(-height / 2, minc.y()));
    maxc = coord(std::min(width / 2, maxc.x()), std::min(height / 2, maxc.y()));
    auto screen_aabb = AABB(
        Point2d(std::min(maxc.x(), minc.x()), std::min(maxc.y(), minc.y())),
        Point2d(std::max(maxc.x(), minc.x()), std::max(maxc.y(), minc.y()))
    );
    // render the triangle that is inside the bounding box
    for (int x = screen_aabb.min().x(); x < screen_aabb.max().x(); x++) {
        for (int y = screen_aabb.min().y(); y < screen_aabb.max().y(); y++) {
            Point2d point(x, y);
            Point3d bary = barycentric(texcoords, point);
            if (inside_triangle(bary)) {
                b.set_pixel_if_deep(coord(x, y), mix_colors(bary, tri.colors), bary[0] * vertices[0].z() + bary[1] * vertices[1].z() + bary[2] * vertices[2].z());
            }
        }
    }
}

// returns the barycentric coordinates of a point in a triangle... all points are relative to the camera
Point3d renderer::barycentric(Point2d vertices[], Point2d point) {
    Point2d v0 = vertices[1] - vertices[0];
    Point2d v1 = vertices[2] - vertices[0];
    Point2d v2 = point - vertices[0];

    double d00 = v0.dot(v0);
    double d01 = v0.dot(v1);
    double d11 = v1.dot(v1);
    double d20 = v2.dot(v0);
    double d21 = v2.dot(v1);

    double denom = d00 * d11 - d01 * d01;
    double v = (d11 * d20 - d01 * d21) / denom;
    double w = (d00 * d21 - d01 * d20) / denom;
    double u = 1.0 - v - w;

    return Point3d(u, v, w);
}