#pragma once

#include <ray.h>
#include <helper.h>
#include "hittable.h"

using namespace glm;

class Camera {
public:
   vec3 position;
   float focal_length, aspect_ratio;
   int img_width, img_height, sample_per_pixel;

private:
   vec2 view_size;
   vec3 pixel_delta_u, pixel_delta_v;
   vec3 viewport_u, viewport_v; 
   vec3 viewport_upper_left, pixel00_pos;

public:   
   Camera(const vec3& position, float focal_length, float aspect_ratio, int img_width, int sample_per_pixel) : 
      position(position), focal_length(focal_length), aspect_ratio(aspect_ratio),
      img_width(img_width), img_height(img_width * aspect_ratio), sample_per_pixel(sample_per_pixel)
      {
         float scale = 2.0;
         view_size = vec2(
            scale * ((float)img_width / img_height),
            scale
         );

         viewport_u = vec3(view_size.x, 0, 0);
         viewport_v = vec3(0, -view_size.y, 0);

         pixel_delta_u = viewport_u / (float)img_width;
         pixel_delta_v = viewport_v / (float)img_height;

         viewport_upper_left = position - vec3(0, 0, focal_length) - viewport_u/2.f - viewport_v/2.f;
         pixel00_pos = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);
      }

   Ray create_ray(int x, int y) {
      vec3 offset(random_double() - 0.5, random_double() - 0.5, 0);

      auto pixel_center = pixel00_pos
         + (float(x + offset.x) * pixel_delta_u)
         + (float(y + offset.y) * pixel_delta_v);

      return Ray(position, pixel_center - position);
   }

   void render(const hittable& world, Image& target) {

      for (int j = 0; j < img_height; j++) 
      {
         std::clog << "\rScanlines remaining: " << (img_height - j) << ' ' << std::flush;
         for (int i = 0; i < img_width; i++)
         {
            vec3 color(0);
            for(int s = 0; s < sample_per_pixel; s++){

               color += ray_color(create_ray(i, j), world);

            }
            target.set_pixel(vec2(i, j), color / (float)sample_per_pixel); 
         }
      }
      std::clog << "\rDone!!                              \n";
   }

private:

   glm::vec3 ray_color(const Ray& r, const hittable& world) const {
      hit_record rec;

        if (world.hit(r, interval(0, infinity), rec)) {
            return 0.5f * (rec.normal + glm::vec3(1));
        }

        vec3 unit_direction = normalize(r.direction);
        float a = 0.5*(unit_direction.y + 1.0);
        return (1.0f-a) * glm::vec3(1.0) + a * glm::vec3(0.5, 0.7, 1.0);
   }

};