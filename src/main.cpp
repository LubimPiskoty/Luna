#include <cstdint>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <iostream>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/common.hpp>
#include <image.h>

int main() {
    
    float scale = 0.5;
    int image_width = 1024*scale;
    int image_height = 1024*scale;

    // Image
    Image mImage(image_width, image_height, 3);

    for (int j = 0; j < image_height; j++) 
    {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++)
        {
            float dist = pow(image_width / 2 - i, 2) + pow(image_height / 2 - j, 2);
            dist = dist / pow(image_width, 2) * 2; // Normalize the distance

            mImage.set_pixel(glm::vec2(i, j), 
                glm::vec3(
                    dist > 0.2 ? 1-dist-0.3 : 0, 
                    0,
                    dist < 0.2 ? dist*10 : 0
                )
            );
        }
    }
    std::clog << "\rDone!!                              \n";


    stbi_write_jpg("render.jpg", mImage.width, mImage.height, mImage.channels, mImage.data, 100);

    return 0;
}