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

#include <thread>
#include <vector>

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
    Scene world;
    auto ground_material = make_shared<lambertian>(vec3(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(vec3(0,-1000,0), 1000, ground_material));

    int floor = 3;
    for (int a = -floor; a < floor; a++) {
        for (int b = -floor; b < floor; b++) {
            auto choose_mat = random_double();
            vec3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = ballRand(1.f);
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = ballRand(0.5f) + vec3(0.5);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<Sphere>(vec3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(vec3(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(vec3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(vec3(4, 1, 0), 1.0, material3));




    Camera camera;
    camera.aspect_ratio      = 4.0 / 5.0;
    camera.img_width       = 1000;
    camera.sample_per_pixel = 200;
    camera.max_depth         = 40;

    camera.vfov     = 80;
    camera.lookfrom = vec3(13,2,3);
    camera.lookat   = vec3(0,0,0);
    camera.vup      = vec3(0,1,0);

    camera.initialize();


    std::filesystem::create_directory("./output/");

    int frame = 180;
    float r = 5.f;

    std::vector<std::thread> threads;
    for (int i = 0; i < frame; i++){
        threads.emplace_back([&camera, world, i, frame, r]() {
            auto a = (float)i/frame * 2.f * pi + pi/2;
            Camera cpy = camera;
            cpy.lookfrom = vec3(cosf(a)*r, 2.f, sinf(a)*r);
            cpy.initialize();
            Image image(cpy.img_width, cpy.img_height, 3);
            single_frame(cpy, world, image);

            std::clog << std::format("Frame {}/{} -> {:.2f}%", i, frame, (float)i/frame*100.f).c_str() << std::endl;
            stbi_write_png(std::format("./output/render-{}.png", i).c_str(), image.width, image.height, image.channels, image.data, 0);
        });
    }

    for(auto& thread : threads)
        thread.join();

    return 0;
}