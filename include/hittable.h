#pragma once

#include <ray.h>

using namespace glm;

class hit_record {
public:
    vec3 position, normal;
    float t;
    bool front_face;
};


class hittable {
public:
    virtual ~hittable() = default;

    virtual bool hit(const Ray& r, double ray_min, double ray_max, hit_record& rec) const = 0;
};