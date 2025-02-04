//
// Created by Boris N on 1/27/25.
//

#ifndef OBJ3_H
#define OBJ3_H
#include "triangle.h"

class obj3 {
public:
    obj3(std::vector<triangle> tris, int num_tris) : tris { std::move(tris) }, num_tris { num_tris } {}

    std::vector<triangle>& get_tris() {
        return tris;
    }

private:
    std::vector<triangle> tris;
    int num_tris;
};

#endif //OBJ3_H
