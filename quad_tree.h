//
// Created by Bartomiej Tempka on 18/05/2023.
//

#ifndef N_BODY_SIM_QUAD_TREE_H
#define N_BODY_SIM_QUAD_TREE_H
#include "gravity_object.h"
#include <vector>
#include "geometry.h"
#include <semaphore>

struct node{
    int children[2][2]{
            {-1,-1},
            {-1,-1}
    };
};

class quad_tree {
private:
    box boundry_box; //boundary box
    int root = 0;
    std::vector<node> nodes;
    std::vector<float> mass;
    std::vector<point> center_of_mass;
    std::vector<float> width;
    std::vector<float> boxes;
    int build_tree(box bound_box, std::vector<gravity_object>::iterator begin, std::vector<gravity_object>::iterator end,int recursion_level);
    void calculate_force_recursive(gravity_object *object, int node);
    const float theta = 0.5; //test
    const float G = 6.67408e-11;
    const float softening_parameter = 0.01;
    float* GLboxes;
    float* GLpoints;
    float update_time = 1;

    std::counting_semaphore<1> semaphore[4] = {std::counting_semaphore<1>(1),std::counting_semaphore<1>(1),std::counting_semaphore<1>(1),std::counting_semaphore<1>(1)};
public:
    quad_tree(std::vector<gravity_object>::iterator begin, std::vector<gravity_object>::iterator end,float* boxes,float* points);
    void calculate_force(gravity_object *object);
    void calculate_force_iter(std::vector<gravity_object>::iterator begin, std::vector<gravity_object>::iterator end);

    void print_tree();
    void update_tree(std::vector<gravity_object>::iterator begin, std::vector<gravity_object>::iterator end);

    std::vector<float>centers;
    void fill_centers(int node);

    void setBoxes(float* boxes) {
        GLboxes = boxes;
    }
};


#endif //N_BODY_SIM_QUAD_TREE_H
