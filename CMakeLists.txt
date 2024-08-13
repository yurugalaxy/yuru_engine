# set to 3.20 for replit compatibility
cmake_minimum_required(VERSION 3.20)
project(yuru_engine)

set(CMAKE_CXX_STANDARD 23)

link_libraries(GL)
link_libraries(glfw)

add_executable(yuru_engine main.cpp
        lib/glad.c
        shader.hpp
        shader_opengl.hpp
        shader_opengl.cpp
        renderer.hpp
        renderer.cpp
        shader.cpp
        camera_flycam.cpp
        camera_flycam.hpp)

target_include_directories(yuru_engine PRIVATE lib)