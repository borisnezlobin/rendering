//
// Created by Boris N on 1/27/25.
//


#include "renderer.h"

void renderer::render_obj(obj3 &obj) {
    render_obj(obj, Point3d(0, 0, 0), Quaterniond(1, 0, 0, 0), Point3d(1, 1, 1));
}

void renderer::render_obj(obj3 &obj, Point3d position, Quaterniond orientation, Point3d scale) {
    for (auto &tri : obj.get_tris()) {
        render_triangle((tri * scale) + position);
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
    render_triangle(tri, nullptr);
}

void renderer::render_triangle(const triangle &tri, std::shared_ptr<Texture> texture) {
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

    Point3d triangle_center = (
        tri.vertices[0] + tri.vertices[1] + tri.vertices[2]
    ) / 3.0;

    // use scanline and edge-intercepts
    // for (int y = screen_aabb.min().y(); y < screen_aabb.max().y(); y++) {
    for (int y = ceil(screen_aabb.min().y()); y < floor(screen_aabb.max().y()); y++) {
        // calculate x-intercept for each edge on this y value
        double intercepts[3] = { -1.0, -1.0, -1.0 };;
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
                intercepts[2] = -1.0;
                break;
            }

            if (fuzzy_compare(texcoords[i].x(), texcoords[j].x())) {
                intercepts[i] = texcoords[i].x();
            } else {
                // find line, calculate x-intercept at y=y
                // (x0, y0) = texcoords[i]; (x1, y1) = texcoords[j].

                double t = (y - texcoords[i].y()) / (texcoords[j].y() - texcoords[i].y());
                intercepts[i] = texcoords[i].x() + t * (texcoords[j].x() - texcoords[i].x());
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
            Point3d world_coords = Point3d(
                bary[0] * vertices[0].x() + bary[1] * vertices[1].x() + bary[2] * vertices[2].x(),
                bary[0] * vertices[0].y() + bary[1] * vertices[1].y() + bary[2] * vertices[2].y(),
                bary[0] * vertices[0].z() + bary[1] * vertices[1].z() + bary[2] * vertices[2].z()
            );
            double dot_prod = (cam.center() - world_coords).dot(triangle_center - world_coords);

            Point2d uv = tri.uvs[0] * bary[0] +
                         tri.uvs[1] * bary[1] +
                         tri.uvs[2] * bary[2];

            color col = red();
            if (tri.tex != nullptr) {
                // get color at tri.tex (1D array) using barycentric coordinates
                double u = uv.x();
                double v = uv.y();
                int x_tex = std::min(int(u * tri.tex_width), tri.tex_width - 1);
                int y_tex = std::min(int((1.0 - v) * tri.tex_height), tri.tex_height - 1); // flip Y

                int pixel_index = (y_tex * tri.tex_width + x_tex) * 3;
                unsigned char r = tri.tex[pixel_index + 0];
                unsigned char g = tri.tex[pixel_index + 1];
                unsigned char b = tri.tex[pixel_index + 2];

                col = color(
                    r / 255.0,
                    g / 255.0,
                    b / 255.0
                );
            }

            b.set_pixel_if_deep(
                coord(x, y),
                col,
                bary[0] * vertices[0].z() + bary[1] * vertices[1].z() + bary[2] * vertices[2].z(),
                dot_prod
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