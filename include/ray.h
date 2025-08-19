#pragma once

#include <glm/vec3.hpp>

using namespace glm;

class Ray {
public:
    vec3 origin, direction;

    Ray(const vec3& origin, const vec3& direction) : origin(origin), direction(direction) {}

};