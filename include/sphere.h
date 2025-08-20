#pragma once

#include <hittable.h>
#include <helper.h>

class Sphere : public hittable {
public:
    glm::vec3 center;
    float radius;
    shared_ptr<material> mat;


    Sphere(const glm::vec3& center, float radius, shared_ptr<material> material) : 
            center(center), radius(fmax(0, radius)), mat(material) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 oc = center - r.origin;
        auto a = glm::length2(r.direction);
        auto h = glm::dot(r.direction, oc);
        auto c = glm::length2(oc) - radius*radius;

        auto discriminant = h*h - a*c;
        if (discriminant < 0.0)
            return false;
        
        auto sqrtd = sqrt(discriminant);

        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.position = r.at(root);
        rec.normal = (rec.position - center) / radius;
        rec.front_face = true;
        rec.mat = mat;

        // Check for backface
        if (dot(rec.normal, r.direction) > 0.0) {
            // ray is inside
            rec.normal = -rec.normal;
            rec.front_face = false;
        }
        return true;
    }
};