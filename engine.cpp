//
// Created by Bartomiej Tempka on 12/06/2023.
//

#include "engine.h"

bool drawQuadTree = false; //idk how to do it better
bool pause = true;
void engine::create_objects() {
    float circle_radius = 0.25;
    point V = point{0,0};
    for (int i = 0; i < number_of_objects-1;i++){
        circle_radius = (((double) rand() / (RAND_MAX))) * 0.1+0.05;
        point vec = point{(((double) rand() / (RAND_MAX))-0.5) *2, (((double) rand() / (RAND_MAX))-0.5)*2};
        vec = vec * (circle_radius / sqrt(vec.x*vec.x + vec.y*vec.y));
        V.x = vec.y;
        V.y = -vec.x;
        V.x = V.x * 0.05;
        V.y = V.y * 0.05;

        objects.push_back(gravity_object(100, vec.x, vec.y, V.x, V.y,i));
    }
    objects.push_back(gravity_object(1000,0, 0, 0, 0, number_of_objects-1));
}

engine::engine() {
    create_objects();
    for(int i =0; i < number_of_objects * 2; i++){
        GLbuffer[i*2] = objects[i].x;
        GLbuffer[i*2+1] = objects[i].y;
    }
    memset(GLbuffer + number_of_objects * 2, 0, number_of_boxes * 8 * sizeof(float));
    QT.create(objects.begin(), objects.end(),GLbuffer + number_of_objects * 2,GLbuffer);
}

int engine::init() {
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        std::cout << "I'm apple machine" << std::endl;
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 1280, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if(glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, number_of_objects*2*sizeof(float) + number_of_boxes*8*sizeof(float), GLbuffer, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);


    std::string vertexShader = "#version 330 core\n"
                               "layout (location = 0) in vec4 position;\n"
                               "void main()\n"
                               "{\n"
                               "   gl_Position = position;\n"
                               "   gl_PointSize = 5;\n"
                               "}\n";
    std::string fragmentShader = "#version 330 core\n"
                                 "out vec4 FragColor;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   FragColor = vec4(1.0f, 1.0f, 1.0f, 0.5f);\n"
                                 "   vec2 circCoord = 2.0 * gl_PointCoord - 1.0;\n"
                                 "   if (dot(circCoord, circCoord) > 0.25) {\n"
                                 "       discard;\n"
                                 "   }\n"
                                 "}\n";

    shaderPoint = CreateShader(vertexShader, fragmentShader);

    std::string fragmentShaderLine = "#version 330 core\n"
                                     "out vec4 FragColor;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   FragColor = vec4(0.0f, 1.0f, 0.0f, 0.5f);\n"
                                     "}\n";

    shaderLine = CreateShader(vertexShader, fragmentShaderLine);
    glEnable(GL_PROGRAM_POINT_SIZE);
    //key callback
    glfwSetKeyCallback(window, key_callback);

    number_of_objects_per_thread = number_of_objects / number_of_threads;
    return 0;
}
int engine::run() {

    while (!glfwWindowShouldClose(window))
    {
        auto start1 = std::chrono::high_resolution_clock::now();
        /* Render her∆∆∆e */
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderPoint);
        glDrawArrays(GL_POINTS, 0, number_of_objects);
        glUseProgram(shaderLine);
        if (drawQuadTree)
            glDrawArrays(GL_LINES, number_of_objects, number_of_boxes*8);
        glBufferData(GL_ARRAY_BUFFER, number_of_objects*2*sizeof(float)+(number_of_boxes * 8)*sizeof(float) , GLbuffer, GL_DYNAMIC_DRAW);
        for(int i = 0; i < number_of_threads; i++){
            threads[i] = std::thread(&quad_tree::calculate_force_iter, &QT, objects.begin() + i * number_of_objects_per_thread, objects.begin() + (i+1) * number_of_objects_per_thread);
        }
        memset(GLbuffer + number_of_objects * 2, 0, number_of_boxes * 8 * sizeof(float));
        for(int i = 0; i < number_of_threads; i++){
            threads[i].join();
        }
        QT.update_tree(objects.begin(),objects.end());


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        do
        {
            glfwPollEvents();
        } while (pause);

        auto stop1 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);
        std::cout << 1/(duration.count()/1000000.0) << " FPS " << std::endl;
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderPoint);
    glDeleteProgram(shaderLine);
    glfwTerminate();
    return 0;
}
unsigned int engine::CreateShader(const std::string &vertexShader, const std::string &fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
unsigned int engine::CompileShader(unsigned int type, const std::string &source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE){
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

void engine::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_Q && action == GLFW_PRESS)
        drawQuadTree = !drawQuadTree;
    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
       pause = !pause;

}
