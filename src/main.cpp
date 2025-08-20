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

using namespace glm;

int main() {
    
    const int resolution = 128*4;
    const float aspect_ratio = 4.f/5.f;
    
    // Image
    Camera camera(vec3(0, 0, 0), 1, aspect_ratio, resolution, 24);
    Image image(camera.img_width, camera.img_height, 3);
    Scene scene;
    scene.add(make_shared<Sphere>(vec3(0, 0, -1), 0.4));
    scene.add(make_shared<Sphere>(vec3(0, -100, -1), 100.0-0.3));

    camera.render(scene, image);

    stbi_write_jpg("render.jpg", image.width, image.height, image.channels, image.data, 100);

    return 0;
}