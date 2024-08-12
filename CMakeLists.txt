cmake_minimum_required(VERSION 3.20)
project(engine)

set(CMAKE_CXX_STANDARD 23)

link_libraries(GL)
link_libraries(glfw)

add_executable(engine main.cpp
        lib/glad.c
        shader.hpp)
