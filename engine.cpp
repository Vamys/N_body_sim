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


void engine::normalize_positions() {
    //normalize point to <-1, 1>
    float denominator1 = (max_cords[0] - max_cords[1]);
    float denominator2 = (max_cords[2] - max_cords[3]);
    for (int i = 0; i < n_objects * 2; i+=2) {
        positions[i] = (positions[i] - max_cords[1]) / denominator1 * 2 - 1;
        positions[i+1] = (positions[i+1] - max_cords[3]) / denominator2 * 2 - 1;
    }
}