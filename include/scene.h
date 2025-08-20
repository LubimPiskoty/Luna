#pragma once

#include <vector>
#include <memory>

#include <hittable.h>

using std::shared_ptr;
using std::make_shared;


class Scene : public hittable {
public:
    std::vector<shared_ptr<hittable>> objects;

    Scene() {}

    void clear() {objects.clear();}

    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    bool hit(const Ray& r, double ray_min, double ray_max, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_max;

        for (const auto& object : objects) {
            if (object->hit(r, ray_min, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};