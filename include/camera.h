#pragma once

#include <ray.h>
#include <helper.h>
#include <hittable.h>
#include <material.h>

using namespace glm;

class Camera {
public:
   float aspect_ratio = 1;
   int img_width = 32, img_height;
   int sample_per_pixel = 100;
   int max_depth = 40;
   float vfov = 60;

   vec3 lookfrom = vec3(0);
   vec3 lookat = vec3(0, 0, -1);
   vec3 vup = vec3(0, 1, 0);

private:
   vec2 view_size;
   vec3 pixel_delta_u, pixel_delta_v;
   vec3 viewport_u, viewport_v; 
   vec3 viewport_upper_left, pixel00_pos;
   vec3 u, v, w;

public:
   void initialize() {
         img_height = img_width * (float)aspect_ratio;

         float focal_length = (lookfrom - lookat).length();
         auto theta = degrees_to_radians(vfov);
         auto h = std::tan(theta/2);
         auto viewport_height = 2 * h * focal_length;
         
         view_size.y = 2 * h * focal_length;
         view_size.x = view_size.y * (double(img_width)/img_height); 

         w = normalize(lookfrom - lookat);
         u = normalize(cross(vup, w));
         v = cross(w, u);

         viewport_u = view_size.x * u;
         viewport_v = view_size.y * -v;

         pixel_delta_u = viewport_u / (float)img_width;
         pixel_delta_v = viewport_v / (float)img_height;

         viewport_upper_left = lookfrom - (focal_length * w) - viewport_u/2.f - viewport_v/2.f;
         pixel00_pos = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);

         
      }

   ray create_ray(int x, int y) {
      vec3 offset(random_double() - 0.5, random_double() - 0.5, 0);

      auto pixel_center = pixel00_pos
         + (float(x + offset.x) * pixel_delta_u)
         + (float(y + offset.y) * pixel_delta_v);

      return ray(lookfrom, pixel_center - lookfrom);
   }

   void render(const hittable& world, Image& target) {
      initialize();
      for (int j = 0; j < img_height; j++) 
      {
         std::clog << "\rScanlines remaining: " << (img_height - j) << ' ' << std::flush;
         for (int i = 0; i < img_width; i++)
         {
            vec3 color(0);
            for(int s = 0; s < sample_per_pixel; s++)
               color += ray_color(create_ray(i, j), max_depth, world);

            target.set_pixel(vec2(i, j), color / (float)sample_per_pixel); 
         }
      }
      std::clog << "\rDone!!                              \n";
   }

private:

   glm::vec3 ray_color(const ray& r, int depth, const hittable& world) const {
      if (depth == 0)
         return vec3(0);

      hit_record rec;

        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            vec3 attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
               return attenuation * ray_color(scattered, depth-1, world);
            return vec3(0);
        }

        vec3 unit_direction = normalize(r.direction);
        float a = 0.5*(unit_direction.y + 1.0);
        return (1.0f-a) * glm::vec3(1.0) + a * glm::vec3(0.5, 0.7, 1.0);
   }

};