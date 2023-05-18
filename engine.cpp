//
// Created by Bartomiej Tempka on 14/04/2023.
//

#include "engine.h"
#include "gravity_object.h"
#include "math.h"

#include <iostream>
engine::engine(){
    if( n_objects == 2){
        objects[0] = gravity_object(1000, -0.25, 0, 0, 0);
        objects[1] = gravity_object(1000, 0.25, 0, 0, 0);
        positions[0] = (float)objects[0].x;
        positions[1] = (float)objects[0].y;
        positions[2] = (float)objects[1].x;
        positions[3] = (float)objects[1].y;
        return;
    }
    for (int i = 0; i < n_objects; i++) {
        objects[i] = gravity_object(1000, (((double) rand() / (RAND_MAX))-0.5) *2, (((double) rand() / (RAND_MAX))-0.5)*2, 0, 0);
    }
    int i = 0;
    while(i<n_objects * 2){
        positions[i] = (float)objects[i/2].x;
        positions[i+1] =(float) objects[i/2].y;
        i+=2;
    }

}

void engine::calc_forces(gravity_object *object1, gravity_object *object2) {
    double r = sqrt(pow(object1->x - object2->x, 2) + pow(object1->y - object2->y, 2));
    if (r <= 0.01) {
//        std::cout << "r = 0" << std::endl;
        return;
    }
    double F = (G * object1->mass * object2->mass) / pow(r, 2);
    double Fx = F * (object2->x - object1->x) / r;
    double Fy = F * (object2->y - object1->y) / r;
    object1->Fx += Fx;
    object1->Fy += Fy;
    object2->Fx -= Fx;
    object2->Fy -= Fy;
}
void engine::update() {
    for (int i = 0; i < n_objects; i++) {
        for (int j = i+1; j < n_objects; j++) {
                calc_forces(&objects[i], &objects[j]);
        }
    }
//    std::cout << "0 - Fx: " << objects[0].Fx << " Fy: " << objects[0].Fy << std::endl;
//    std::cout << "1 - Fx: " << objects[1].Fx << " Fy: " << objects[1].Fy << std::endl;
    for (int i = 0; i < n_objects; i++) {
        objects[i].vx += objects[i].Fx / objects[i].mass * update_time;
        objects[i].vy += objects[i].Fy / objects[i].mass * update_time;
        objects[i].x += objects[i].vx * update_time;
        objects[i].y += objects[i].vy * update_time;
        objects[i].Fx = 0;
        objects[i].Fy = 0;
    }
    int i = 0;
    while(i<n_objects * 2){
        positions[i] = (float)objects[i/2].x;
        positions[i+1] = (float)objects[i/2].y;
        i+=2;
    }
}