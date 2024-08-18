#include "window.hpp"

#include <iostream>

namespace Lyonesse
{
  GLFWwindow* InitialiseWindow(const int width, const int height, const char* title)
  {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (window == nullptr)
    {
      std::cout << "Failed to create GLFW window.\n";
      glfwTerminate();
    }
    return window;
  }
}
