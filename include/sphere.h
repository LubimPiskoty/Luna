#pragma once

#include <hittable.h>
#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>

class Sphere : public hittable {
public:
    glm::vec3 center;
    float radius;


    Sphere(const glm::vec3& center, float radius) : center(center), radius(fmax(0, radius)) {}

    bool hit(const Ray& r, double ray_min, double ray_max, hit_record& rec) const override {
        vec3 oc = center - r.origin;
        auto a = glm::length2(r.direction);
        auto h = glm::dot(r.direction, oc);
        auto c = glm::length2(oc) - radius*radius;

        auto discriminant = h*h - a*c;
        if (discriminant < 0.0)
            return false;
        
        auto sqrtd = sqrt(discriminant);

        auto root = (h - sqrtd) / a;
        if (root <= ray_min || root >= ray_max) {
            root = (h + sqrtd) / a;
            if (root <= ray_min || root >= ray_max)
                return false;
        }

        rec.t = root;
        rec.position = r.at(root);
        rec.normal = (rec.position - center) / radius;
        rec.front_face = true;

        // Check for backface
        if (dot(rec.normal, r.direction) > 0.0) {
            // Ray is inside
            rec.normal = -rec.normal;
            rec.front_face = false;
        } 
    }
};