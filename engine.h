//
// Created by Bartomiej Tempka on 12/06/2023.
//

#ifndef N_BODY_SIM_ENGINE_H
#define N_BODY_SIM_ENGINE_H
#include <vector>
#include "gravity_object.h"
#include "geometry.h"
#include "quad_tree.h"
#include <thread>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class engine {
private:
    std::vector<gravity_object> objects;
    void create_objects();
    static const int number_of_objects = 5000;
    static const int number_of_boxes = number_of_objects;
    float GLbuffer[2*number_of_objects + 8*number_of_boxes];
    GLFWwindow* window;
    unsigned int shaderPoint;
    unsigned int shaderLine;
    static const int number_of_threads = 8;
    std::thread threads[number_of_threads];
    unsigned int VAO, VBO;
    int number_of_objects_per_thread;
    quad_tree QT = quad_tree();
    static unsigned int CompileShader( unsigned int type ,const std::string& source);
    static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
    engine();
    int init();
    int run();
};


#endif //N_BODY_SIM_ENGINE_H
