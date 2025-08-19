#include <cstdint>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <iostream>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <image.h>
#include <ray.h>
#include <camera.h>

using namespace glm;

bool hit_sphere(const vec3& center, double radius, const Ray& r) {
    vec3 oc = center - r.origin;
    auto a = dot(r.direction, r.direction);
    auto b = -2.0 * dot(r.direction, oc);
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    return (discriminant >= 0);
}

vec3 do_fragment(int x, int y, Camera camera) {
    // Simple gradient
    Ray ray = camera.create_ray(x, y);

    if (hit_sphere(vec3(0, 0, -1), 0.5, ray))
        return vec3(1, 0, 0);

    auto color = mix(vec3(1, 1, 1), vec3(0.5, 0.7, 1.0), normalize(ray.direction).y * 0.5 + 0.5);
    return color;
}


int main() {
    
    const int resolution = 1024*1.0;
    const float aspect_ratio = 4.f/5.f;

    const int img_width = resolution;
    const int img_height = img_width / aspect_ratio;

    // Image
    Image mImage(img_width, img_height, 3);
    Camera mCamera(vec3(0, 0, 0), 1, aspect_ratio, vec2(img_width, img_height));

    for (int j = 0; j < img_height; j++) 
    {
        std::clog << "\rScanlines remaining: " << (img_height - j) << ' ' << std::flush;
        for (int i = 0; i < img_width; i++)
        {
            mImage.set_pixel(vec2(i, j), do_fragment(i, j, mCamera));
        }
    }
    std::clog << "\rDone!!                              \n";


    stbi_write_jpg("render.jpg", mImage.width, mImage.height, mImage.channels, mImage.data, 100);

    return 0;
}