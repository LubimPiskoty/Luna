#pragma once

#include <hittable.h>
#include <helper.h>

class material {
  public:
    virtual ~material() = default;

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        return false;
    }
};

class lambertian : public material {
  public:
    lambertian(const vec3& albedo) : albedo(albedo) {}

    bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override {
        auto scatter_direction = rec.normal + random_unit_vector();
        if (near_zero(scatter_direction))
            scatter_direction = rec.normal;

        scattered = ray(rec.position, scatter_direction);
        attenuation = albedo;
        return true;
    }

  private:
    vec3 albedo;
};


class metal : public material {
  public:
    metal(const vec3& albedo) : albedo(albedo) {}

    bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)
    const override {
        vec3 reflected = reflect(r_in.direction, rec.normal);
        scattered = ray(rec.position, reflected);
        attenuation = albedo;
        return true;
    }

  private:
    vec3 albedo;
};