#pragma once

#include <glm/vec3.hpp>

using namespace glm;

class ray {
public:
    vec3 origin, direction;

    ray() = default;
    ray(const vec3& origin, const vec3& direction) : origin(origin), direction(normalize(direction)) {}

    const vec3& at(float t) const {
        return origin+direction*t;
    }

};