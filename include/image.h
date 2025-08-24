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
        delete[] data;
    }

    inline double linear_to_gamma(double linear_component)
    {
        if (linear_component > 0)
            return std::sqrt(linear_component);

        return 0;
    }
    
    void set_pixel(const glm::vec2& position, const glm::vec3& color) {
        if (position.x >= this->width || position.y >= this->height || 
            position.x < 0 || position.y < 0) {
            std::clog << "[Error]\t Invalid `set_pixel` position! (" << position.x << ", " << position.y << ")" << std::endl;
            return;
        }

        // Edit the colors
        // Add contrast
        float contrast = 1.5f;
        float brightness = 1.f;
        // Translate the [0,1] component values to the byte range [0,255].
        static const interval intensity(0.000, 0.999);
        int rbyte = int(256 * intensity.clamp(linear_to_gamma(color.r)));
        int gbyte = int(256 * intensity.clamp(linear_to_gamma(color.g))); 
        int bbyte = int(256 * intensity.clamp(linear_to_gamma(color.b)));

        uint64_t pixel_start = (this->width * (int)position.y + (int)position.x) * this->channels;
        this->data[pixel_start + 0] = rbyte; 
        this->data[pixel_start + 1] = gbyte;
        this->data[pixel_start + 2] = bbyte;
    }
};