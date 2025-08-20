#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp> // glm::vec3
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/random.hpp>

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

#include <interval.h>

// C++ Std Usings

using std::make_shared;
using std::shared_ptr;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;
const float epsilon = 1.0E-3F; 

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

inline glm::vec3 random_unit_vector() {
    while (true) {
        glm::vec3 p = glm::ballRand(1.f);
        auto lensq = glm::length2(p);
        if (1e-160 < lensq && lensq <= 1)
            return p / sqrtf(lensq);
    }
}

inline glm::vec3 random_on_hemisphere(const glm::vec3& normal) {
    glm::vec3 on_unit_sphere = random_unit_vector(); 

    if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

inline bool near_zero(const glm::vec3& v) {
    return glm::length2(v) < powf(std::numeric_limits<float>::epsilon(), 2);
}

inline glm::vec3 reflect(const glm::vec3& v, const glm::vec3& n) {
    return v - 2.0f*dot(v,n)*n;
}