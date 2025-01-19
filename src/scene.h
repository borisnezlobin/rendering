//
// Created by Boris N on 1/18/25.
//

#ifndef SCENE_H
#define SCENE_H
#include "obj3.h"

using std::make_shared;
using std::shared_ptr;

class scene : public obj3 {
public:
    std::vector<shared_ptr<obj3>> objects;

    scene() = default;
    explicit scene(const shared_ptr<obj3>& obj) { add(obj); }

    void add(const shared_ptr<obj3>& obj) {
        objects.push_back(obj);
    }

    bool hit(ray &r, double tmin, double tmax, hit_record &rec) const override {
        bool hit_anything = false;
        hit_record tmp;
        auto closest = tmax;

        for (const auto& obj: objects) {
            if (obj->hit(r, tmin, tmax, tmp)) {
                hit_anything = true;

                if (tmp.t < closest) {
                    closest = tmp.t;
                    rec = tmp;
                }
            }
        }

        return hit_anything;
    }
};

#endif //SCENE_H
