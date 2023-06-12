cmake_minimum_required(VERSION 3.24)
project(N_body_sim)

set(CMAKE_CXX_STANDARD 17)

add_executable(N_body_sim main.cpp gravity_object.cpp gravity_object.h quad_tree.h quad_tree.cpp geometry.h engine.h engine.cpp)
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(N_body_sim glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(N_body_sim OpenGL::GL)
find_package(GLEW REQUIRED)
target_link_libraries(N_body_sim GLEW::GLEW)