#pragma once

#include <ray.h>
#include <glm/vec3.hpp>

using namespace glm;

class Camera {
public:
   vec3 position;
   float focal_length, aspect_ratio;

private:
   vec2 view_size;
   vec3 pixel_delta_u, pixel_delta_v;
   vec3 viewport_u, viewport_v; 
   vec3 viewport_upper_left, pixel00_pos;

public:   
   Camera(const vec3& position, float focal_length, float aspect_ratio, vec2 img_size) : 
      position(position), focal_length(focal_length), aspect_ratio(aspect_ratio) 
      {
         float scale = 2.0;
         view_size = vec2(
            scale * ((float)img_size.x / img_size.y),
            scale
         );

         viewport_u = vec3(view_size.x, 0, 0);
         viewport_v = vec3(0, -view_size.y, 0);

         pixel_delta_u = viewport_u / img_size.x;
         pixel_delta_v = viewport_v / img_size.y;

         viewport_upper_left = position - vec3(0, 0, focal_length) - viewport_u/2.f - viewport_v/2.f;
         pixel00_pos = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);
      }

   Ray create_ray(int x, int y) {
      auto pixel_center = pixel00_pos + ((float)x * pixel_delta_u) + ((float)y * pixel_delta_v);
      return Ray(position, pixel_center - position);
   }

};