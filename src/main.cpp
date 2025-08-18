#include <cstdint>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>

using namespace std;

int main() {
    
    int image_width = 256;
    int image_height = 256;

    // Image
    uint8_t* img = new uint8_t[image_width * image_height * 3];

    int index = 0;
    for (int j = image_height - 1; j >= 0; --j)
    {
        for (int i = 0; i < image_width; ++i)
            {
            auto r = double(i) / (double)(image_width-1);
            auto g = double(j) / (double)(image_height-1);
            auto b = 0.0;

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);
            
            img[index++] = ir;
            img[index++] = ig;
            img[index++] = ib;
        }
    }

    stbi_write_jpg("img.jpg", image_width, image_height, 3, img, 100);

    return 0;
}