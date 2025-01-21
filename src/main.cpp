//
// Created by Boris N on 1/14/25.
//

#include "main.h"

#include <iostream>

#include "camera.h"
#include "obj3.h"
#include "scene.h"
#include "sphere.h"
#include "util.h"


int main() {
    camera cam(300, 16.0 / 9.0);
    scene world;

    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));

    cam.render(world);
}
