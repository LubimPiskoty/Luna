#include <cstdint>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp> // glm::vec3
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <image.h>
#include <ray.h>
#include <camera.h>
#include <sphere.h>
#include <scene.h>

using namespace glm;


vec3 do_fragment(int x, int y, Camera camera, const Scene& scene) {
    // Simple gradient
    Ray ray = camera.create_ray(x, y);
    
    hit_record rec;
    if (scene.hit(ray, 0, 10, rec)) {
        return rec.normal/2.f + vec3(0.5);
    }

    auto color = mix(vec3(1, 1, 1), vec3(0.5, 0.7, 1.0), normalize(ray.direction).y * 0.5 + 0.5);
    return color;
}


int main() {
    
    const int resolution = 1024*.5;
    const float aspect_ratio = 4.f/5.f;

    const int img_width = resolution;
    const int img_height = img_width / aspect_ratio;

    // Image
    Image mImage(img_width, img_height, 3);
    Camera mCamera(vec3(0, 0, 0), 1, aspect_ratio, vec2(img_width, img_height));
    Scene scene;
    scene.add(make_shared<Sphere>(vec3(0, 0, -1), 0.4));
    scene.add(make_shared<Sphere>(vec3(0, -100, -1), 100));

    for (int j = 0; j < img_height; j++) 
    {
        std::clog << "\rScanlines remaining: " << (img_height - j) << ' ' << std::flush;
        for (int i = 0; i < img_width; i++)
        {
            mImage.set_pixel(vec2(i, j), do_fragment(i, j, mCamera, scene));
        }
    }
    std::clog << "\rDone!!                              \n";


    stbi_write_jpg("render.jpg", mImage.width, mImage.height, mImage.channels, mImage.data, 100);

    return 0;
}