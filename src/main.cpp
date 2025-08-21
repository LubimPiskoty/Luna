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
#include <format>
#include <filesystem>

using namespace std::chrono;
using namespace glm;

void single_frame(Camera& camera, const Scene& scene, Image& image) {
    auto start = high_resolution_clock::now();
    camera.render(scene, image);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    std::cout << "Rendering took: " << duration.count() << "sec" << std::endl;
}

int main() {
    Camera camera;
    camera.lookfrom = vec3(0, 1.f, 1.f);
    camera.lookat = vec3(0, 0, -1);

    camera.img_width = 128*4;
    camera.aspect_ratio = 4.f/5.f;
    camera.sample_per_pixel = 20;
    camera.max_depth = 20;
    camera.vfov = 60;
    camera.initialize();

    Image image(camera.img_width, camera.img_height, 3);

    auto mat_light = make_shared<lambertian>(vec3(0.75f));
    auto mat_red = make_shared<lambertian>(vec3(1, 0, 0));
    auto mat_green = make_shared<lambertian>(vec3(0, 1, 0));

    auto mat_mirror = make_shared<metal>(vec3(1.0), 0.0);
    auto mat_matte_green_mirror = make_shared<metal>(vec3(0.8, 1., 0.8), 0.2);

    auto mat_glass = make_shared<dielectric>(1.3);

    Scene scene;
    scene.add(make_shared<Sphere>(vec3(0, 0.05, -1), 0.4, mat_mirror));
    scene.add(make_shared<Sphere>(vec3(0.7, -.1, -1), 0.3, mat_red));
    scene.add(make_shared<Sphere>(vec3(-0.7,-.1, -1), 0.3, mat_matte_green_mirror));
    scene.add(make_shared<Sphere>(vec3(0.3, 0.1, -0.4), 0.2, mat_glass));
    scene.add(make_shared<Sphere>(vec3(0, -100, -1), 100.0-0.38, mat_light));

    std::filesystem::create_directory("./output/");

    int frame = 60;
    float r = 2.f;

    for (int i = 0; i < frame; i++){
        auto a = (float)i/frame * 2.f * pi;
        camera.lookfrom = vec3(cosf(a)*r, 1, sinf(a)*r);

        single_frame(camera, scene, image);

        std::clog << std::format("Frame {}/{} -> {}", i, frame, (float)i/frame*100.f).c_str() << std::endl;
        stbi_write_jpg(std::format("./output/render-{}.jpg", i).c_str(), image.width, image.height, image.channels, image.data, 100);
    }

    return 0;
}