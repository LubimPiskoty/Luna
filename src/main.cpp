#include <cstdint>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <iostream>

#include <helper.h>
#include <image.h>
#include <ray.h>
#include <camera.h>
#include <sphere.h>
#include <scene.h>
#include <material.h>

#include <chrono>

using namespace std::chrono;
using namespace glm;

int main() {
    Camera camera;
    camera.position = vec3(0, 0, 0.5f);
    camera.img_width = 128*10;
    camera.aspect_ratio = 4.f/5.f;
    camera.sample_per_pixel = 100;
    camera.max_depth = 20;
    camera.focal_length = 1.f;
    camera.initialize();

    Image image(camera.img_width, camera.img_height, 3);

    auto mat_light = make_shared<lambertian>(vec3(0.8f));
    auto mat_red = make_shared<lambertian>(vec3(1, 0, 0));
    auto mat_green = make_shared<lambertian>(vec3(0, 1, 0));

    auto mat_mirror = make_shared<metal>(vec3(1.0));

    Scene scene;
    scene.add(make_shared<Sphere>(vec3(0, 0, -1), 0.4, mat_mirror));
    scene.add(make_shared<Sphere>(vec3(1, 0, -1), 0.4, mat_red));
    scene.add(make_shared<Sphere>(vec3(-1, 0, -1), 0.4, mat_green));
    scene.add(make_shared<Sphere>(vec3(0, -100, -1), 100.0-0.35, mat_light));

    auto start = high_resolution_clock::now();
    camera.render(scene, image);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    std::cout << "Rendering took: " << duration.count() << "sec" << std::endl;

    stbi_write_jpg("render.jpg", image.width, image.height, image.channels, image.data, 100);

    return 0;
}