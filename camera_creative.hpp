//
// Created by kae on 13/08/24.
//

#pragma once

#include <GLFW/glfw3.h>

namespace Yuru
{
  struct CameraAngles
  {
    glm::vec3 cameraPos {};
    glm::vec3 cameraUp {};
    glm::vec3 cameraFront {};
    float yaw {};
    float pitch {};
    bool clampedY {};
  };

  struct MousePosition
  {
    float mouseX {};
    float mouseY {};
    bool firstMouseMove {};
    bool mouseActive {};
  };

  class CreativeCamera
  {
  public:
    CreativeCamera(float screenWidth, float screenHeight, float fov);
    ~CreativeCamera();
    void ProcessInput(GLFWwindow* window, float deltaTime);
    void ProcessMouse(double xPos, double yPos);
    void ProcessMouseButtons(GLFWwindow* window, int button, int action, int mods);

  public:
    CameraAngles angles {};
    MousePosition mousepos {};
    float FOV {};
  };
}
