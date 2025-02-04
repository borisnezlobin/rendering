//
// Created by Boris N on 1/21/25.
//

#ifndef MATERIAL_H
#define MATERIAL_H

#include "obj3.h"
#include "ray.h"

class material {
public:
    virtual ~material() = default;

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        return false;
    }
};

class lambertian : public material {
public:
    explicit lambertian(const color& albedo) : albedo(albedo) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        auto scatter_direction = rec.normal + random_unit_vector();
        if (scatter_direction.near_zero()) scatter_direction = rec.normal;
        scattered = ray(rec.point, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};

class metal : public material {
public:
    explicit metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        reflected += random_unit_vector() * fuzz;
        scattered = ray(rec.point, reflected);
        attenuation = albedo;
        return dot(reflected, rec.normal) > 0;
    }

private:
    color albedo;
    double fuzz;
};

#endif //MATERIAL_H
