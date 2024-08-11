cmake_minimum_required(VERSION 3.30)
project(engine)

set(CMAKE_CXX_STANDARD 23)

link_libraries(GL)
link_libraries(glfw)

add_executable(engine main.cpp
        glad.c
        shader.hpp)
