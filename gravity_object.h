//
// Created by Bartomiej Tempka on 14/04/2023.
//

#ifndef N_BODY_SIM_GRAVITY_OBJECT_H
#define N_BODY_SIM_GRAVITY_OBJECT_H


class gravity_object {
public:
    gravity_object();
    gravity_object(double mass, double x, double y, double vx, double vy,int index);
    double mass;
    double x;
    double y;
    double vx;
    double vy;
    double Fx;
    double Fy;
    int index;
};


#endif //N_BODY_SIM_GRAVITY_OBJECT_H
