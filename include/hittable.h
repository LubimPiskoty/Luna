#pragma once

#include <ray.h>

using namespace glm;

class material;

class hit_record {
public:
    vec3 position, normal;
    float t;
    bool front_face;
    shared_ptr<material> mat;
};


class hittable {
public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};