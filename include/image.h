#pragma once

#include <iostream>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Image {

public:
    uint8_t* data;
    int width, height, channels;

    Image(int width, int height, int channels) {
        this->width = width;
        this->height = height;
        this->channels = channels;

        data = new uint8_t[width * height * channels];
    }

    ~Image() {
        delete data;
    }
    
    void set_pixel(const glm::vec2& position, const glm::vec3& color) {
        if (position.x >= this->width || position.y >= this->height || 
            position.x < 0 || position.y < 0) {
            std::clog << "[Error]\t Invalid `set_pixel` position! (" << position.x << ", " << position.y << ")" << std::endl;
            return;
        }

        // If the color values is out of bounds draw a pink pixel
        if (color.r < 0 || color.g < 0 || color.b < 0 ||
            color.r > 1 || color.g > 1 || color.b > 1 ) {
            set_pixel(position, glm::vec3(1, 0, 1));
            return;
        }

        uint64_t pixel_start = (this->width * (int)position.y + (int)position.x) * this->channels;

        const float max_val = 255.99;
        this->data[pixel_start + 0] = uint8_t(color.r * max_val);
        this->data[pixel_start + 1] = uint8_t(color.g * max_val);
        this->data[pixel_start + 2] = uint8_t(color.b * max_val);
    }
};