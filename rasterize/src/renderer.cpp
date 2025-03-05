//
// Created by Boris N on 1/27/25.
//


#include "renderer.h"

void renderer::render_obj(obj3 &obj) {
    for (auto &tri : obj.get_tris()) {
        render_triangle(tri);
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
            1,
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
        1,
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

    // todo: check if all the vertices are hidden

    if (!on_screen) return;

    Point2d minc = Point2d(
        std::min(texcoords[0].x(), std::min(texcoords[1].x(), texcoords[2].x())),
        std::min(texcoords[0].y(), std::min(texcoords[1].y(), texcoords[2].y()))
    );
    Point2d maxc = Point2d(
        std::max(texcoords[0].x(), std::max(texcoords[1].x(), texcoords[2].x())),
        std::max(texcoords[0].y(), std::max(texcoords[1].y(), texcoords[2].y()))
    );

    // clip the bounding box to the screen
    minc = Point2d(std::max(-width / 2.0, minc.x()), std::max(-height / 2.0, minc.y()));
    maxc = Point2d(std::min((width - 1) / 2.0, maxc.x()), std::min((height - 1) / 2.0, maxc.y()));
    auto screen_aabb = AABB(
        Point2d(std::min(maxc.x(), minc.x()), std::min(maxc.y(), minc.y())),
        Point2d(std::max(maxc.x(), minc.x()), std::max(maxc.y(), minc.y()))
    );

    // use scanline and edge-intercepts
    for (int y = screen_aabb.min().y(); y <= screen_aabb.max().y(); y++) {
        // calculate x-intercept for each edge on this y value
        double intercepts[3];
        for (int i = 0; i < 3; i++) {
            int j = (i + 1) % 3;
            if ((texcoords[i].y() > y && texcoords[j].y() > y) || (texcoords[i].y() < y && texcoords[j].y() < y)) {
                intercepts[i] = -1;
                continue;
            }
            if (fuzzy_compare(texcoords[i].y(), texcoords[j].y())) {
                // since the entire width of the triangle is on this line, we can just draw a line
                // but I'm lazy, so instead we're gonna fake the endpoints of the line
                intercepts[0] = texcoords[i].x();
                intercepts[1] = texcoords[j].x();
                break;
            }

            if (fuzzy_compare(texcoords[i].x(), texcoords[j].x())) {
                intercepts[i] = texcoords[i].x();
            } else {
                // find line, calculate x-intercept at y=y
                // (x0, y0) = texcoords[i]; (x1, y1) = texcoords[j].
                double slope = (texcoords[i].y() - texcoords[j].y()) / (texcoords[i].x() - texcoords[j].x());
                intercepts[i] = (y - texcoords[i].y()) / slope + texcoords[i].x();
            }
        }


        double min_intercept = infinity;
        double max_intercept = -infinity;
        for (int i = 0; i < 3; i++) {
            if (intercepts[i] == -1) continue;
            min_intercept = std::min(min_intercept, intercepts[i]);
            max_intercept = std::max(max_intercept, intercepts[i]);
        }

        if (min_intercept > screen_aabb.max().x() || max_intercept < screen_aabb.min().x()) {
            continue;
        }

        // clamp intercepts to the screen
        min_intercept = std::max(min_intercept, screen_aabb.min().x());
        max_intercept = std::min(max_intercept, screen_aabb.max().x());

        // round up both intercepts
        int min_intercept_i = std::ceil(min_intercept);
        int max_intercept_i = std::floor(max_intercept);

        // now we can draw the horizontal line
        int x = max_intercept_i;
        do {
            Point3d bary = barycentric(texcoords, Point2d(x, y));
            b.set_pixel_if_deep(
                coord(x, y),
                mix_colors(bary, tri.colors),
                bary[0] * vertices[0].z() + bary[1] * vertices[1].z() + bary[2] * vertices[2].z()
            );
            x--;
        } while (x >= min_intercept_i);
    }
}

// returns the barycentric coordinates of a point in a triangle
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