//
// Created by Bartomiej Tempka on 18/05/2023.
//

#include "quad_tree.h"
#include "gravity_object.h"
#include <limits>
#include "geometry.h"
#include <vector>
#include <iostream>
#include <algorithm>

quad_tree::quad_tree(std::vector<gravity_object>::iterator begin, std::vector<gravity_object>::iterator end, float* boxes) {
    GLboxes = boxes;
    root = build_tree(bbox(begin,end),begin, end);
    for (auto i = 0; i < nodes.size(); i++)
        std::cout << i << " " << center_of_mass[i].x << " " << center_of_mass[i].y << " " << mass[i] << " " << width[i] << std::endl;
}

int quad_tree::build_tree(box const bound_box, std::vector<gravity_object>::iterator begin,
                          std::vector<gravity_object>::iterator end) {
    if (begin == end)
        return -1;

    int result = nodes.size();
    nodes.emplace_back();
    mass.emplace_back();
    center_of_mass.emplace_back();
    width.emplace_back();
    if (std::equal(begin, end, begin+1, [](gravity_object const & a, gravity_object const & b){return a.y == b.y && a.x == b.x;})){
        std::cout << "equal" << std::endl;
        exit(1);
    }
    if (begin +1 == end) {
        mass[result] = begin->mass;
        center_of_mass[result] = point{begin->x, begin->y};
        width[result] = 0;
        return result;}

    point center = middle(bound_box.bottom_left, bound_box.top_right);

    *GLboxes = bound_box.bottom_left.x;
    GLboxes++;
    *GLboxes = center.y;
    GLboxes++;
    *GLboxes = bound_box.top_right.x;
    GLboxes++;
    *GLboxes = center.y;
    GLboxes++;
    *GLboxes = center.x;
    GLboxes++;
    *GLboxes = bound_box.bottom_left.y;
    GLboxes++;
    *GLboxes = center.x;
    GLboxes++;
    *GLboxes = bound_box.top_right.y;
    GLboxes++;

    auto split_y =std::partition(
            begin,
            end,
            [center](gravity_object const & p){ return p.y < center.y; }
    );
    auto split_x_lower =std::partition(
            begin,
            split_y,
            [center](gravity_object const & p){ return p.x < center.x; }
    );
    auto split_x_upper =std::partition(
            split_y,
            end,
            [center](gravity_object const & p){ return p.x < center.x; }
    );
    /*
     * [begin, split_x_lower) are in the lower-left quadrant
     * [split_x_lower, split_y) are in the lower-right quadrant
     * [split_y, split_x_upper) are in the upper-left quadrant
     * [split_x_upper, end) are in the upper-right quadrant
     */
    nodes[result].children[0][0] = build_tree(
            box{bound_box.bottom_left, center},
            begin,
            split_x_lower
    );
    nodes[result].children[0][1] = build_tree(
            box{point{center.x, bound_box.bottom_left.y}, point{bound_box.top_right.x, center.y}},
            split_x_lower,
            split_y
    );
    nodes[result].children[1][0] = build_tree(
            box{point{bound_box.bottom_left.x, center.y}, point{center.x, bound_box.top_right.y}},
            split_y,
            split_x_upper
    );
    nodes[result].children[1][1] = build_tree(
            box{center, bound_box.top_right},
            split_x_upper,
            end
    );
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j <2 ; ++j) {
            if (nodes[result].children[i][j] == -1)
                continue;
            mass[result] += mass[nodes[result].children[i][j]];
            center_of_mass[result] += center_of_mass[nodes[result].children[i][j]] * mass[nodes[result].children[i][j]];
        }
    }
    center_of_mass[result] /= mass[result];

    width[result] = std::max(
            std::max(
                    std::max(
                            std::abs(center_of_mass[result].x - bound_box.bottom_left.x),
                            std::abs(center_of_mass[result].x - bound_box.top_right.x)
                    ),
                    std::max(
                            std::abs(center_of_mass[result].y - bound_box.bottom_left.y),
                            std::abs(center_of_mass[result].y - bound_box.top_right.y)
                    )
            ),
            std::numeric_limits<double>::epsilon()
    );

    return result;

}
void quad_tree::print_tree() {
    for (int i = 0; i < nodes.size(); ++i) {
        std::cout << "Node " << i << " has children: " << nodes[i].children[0][0] << " " << nodes[i].children[0][1] << " " << nodes[i].children[1][0] << " " << nodes[i].children[1][1]
                  << "|" << mass[i] << "|" << center_of_mass[i] << std::endl;
    }
}

void quad_tree::calculate_force(gravity_object *object) {
    calculate_force(object, root);
}
void quad_tree::calculate_force(gravity_object *object,int node) {
    //what if node is the object itself?
    if(object->x == center_of_mass[node].x && object->y == center_of_mass[node].y){
        return;
    }
    float distance = sqrt(pow(object->x - center_of_mass[node].x,2) + pow(object->y - center_of_mass[node].y,2)) + softening_parameter;

    if(width[node]/distance < theta){
        double force = (G * mass[node] * object->mass) / pow(distance,2);
        object->Fx += force * (center_of_mass[node].x - object->x) / distance;
        object->Fy += force * (center_of_mass[node].y - object->y) / distance;
    }
    else{
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j <2 ; ++j) {
                if (nodes[node].children[j][i] == -1)
                    continue;
                calculate_force(object, nodes[node].children[j][i]);
            }
        }
    }
}

void quad_tree::update_tree(std::vector<gravity_object>::iterator begin, std::vector<gravity_object>::iterator end) {
    nodes.clear();
    mass.clear();
    center_of_mass.clear();
    width.clear();
    root = build_tree(bbox(begin,end),begin, end);
}

void quad_tree::fill_centers(int node) {
    if (nodes[node].children[0][0] == -1 && nodes[node].children[0][1] == -1 && nodes[node].children[1][0] == -1 && nodes[node].children[1][1] == -1){
        return;
    }
    centers.push_back(center_of_mass[node].x);
    centers.push_back(center_of_mass[node].y);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j <2 ; ++j) {
            if (nodes[node].children[i][j] == -1)
                continue;
            fill_centers(nodes[node].children[i][j]);
        }
    }
}