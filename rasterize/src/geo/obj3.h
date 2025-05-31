//
// Created by Boris N on 1/27/25.
//

#ifndef OBJ3_H
#define OBJ3_H

#include "../util/Texture.h"
#include "triangle.h"

class obj3 {
public:

    obj3(std::vector<triangle> tris, int num_tris) : tris { std::move(tris) }, num_tris { num_tris } {}

    std::vector<triangle>& get_tris() {
        return tris;
    }

    static obj3 load_model(std::string str);
    std::vector<triangle> tris;

private:
    int num_tris;
    std::shared_ptr<Texture> texture; // class Texture must support sample(u, v)
};



#endif //OBJ3_H
