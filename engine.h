//
// Created by Bartomiej Tempka on 14/04/2023.
//

#ifndef N_BODY_SIM_ENGINE_H
#define N_BODY_SIM_ENGINE_H
#include "gravity_object.h"

class engine {
private:
    const static unsigned int n_objects = 5;
    gravity_object objects[n_objects];
    constexpr static const double softening_parameter = 0.001;
    constexpr static const double G = 6.67408e-11;
    constexpr static const double update_time = 1;
    float max_cords [4] = {-1, 1, -1, 1}; // x_min, x_max, y_min, y_max
    bool normalize = false;
public:
    engine();
    float positions [n_objects*2];
    void normalize_positions();

};


#endif //N_BODY_SIM_ENGINE_H
