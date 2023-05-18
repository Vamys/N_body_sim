//
// Created by Bartomiej Tempka on 14/04/2023.
//

#ifndef N_BODY_SIM_ENGINE_H
#define N_BODY_SIM_ENGINE_H
#include "gravity_object.h"

class engine {
private:
    void calc_forces(gravity_object* object1, gravity_object* object2);
    const static unsigned int n_objects = 2500;
    gravity_object objects[n_objects];

    constexpr static const double G = 6.67408e-11;
    constexpr static const double update_time = 1;
public:
    engine();
//    engine(unsigned int n_objects);
//    engine(gravity_object objects[]);
    void update();
    float positions [n_objects*2];
    const unsigned int get_n_objects() {return n_objects;}

};


#endif //N_BODY_SIM_ENGINE_H
