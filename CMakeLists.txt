# set to 3.20 for replit compatibility
cmake_minimum_required(VERSION 3.20)
project(yuru_engine)

set(CMAKE_CXX_STANDARD 23)

link_libraries(GL)
link_libraries(glfw)

add_executable(yuru_engine main.cpp
        lib/glad.c
        shader.hpp)

target_include_directories(yuru_engine PRIVATE         lib)