//
// Created by Bartomiej Tempka on 14/04/2023.
//

#include "gravity_object.h"
#include "math.h"

gravity_object::gravity_object(double mass, double x, double y, double vx, double vy,int index) : mass(mass) {
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
    this->Fx = 0;
    this->Fy = 0;
    this->index = index;
}

gravity_object::gravity_object() : mass(1){
    this->x = 0;
    this->y = 0;
    this->vx = 0;
    this->vy = 0;
    this->Fx = 0;
    this->Fy = 0;
    this->index = 0;

}



