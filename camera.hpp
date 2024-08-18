//
// Created by kae on 18/08/24.
//

#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "base.hpp"

namespace Lyonesse
{
  struct CameraAngles
  {
    glm::vec3 cameraPos {};
    glm::vec3 cameraUp {};
    glm::vec3 cameraFront {};
    float yaw {};
    float pitch {};
  };

  struct MousePosition
  {
    float mouseX {};
    float mouseY {};
    bool firstMouseMove {};
    bool mouseActive {};
  };
  class CreativeCamera;
  class Camera2D;

  class Camera
  {
  public:
    static Shared<Camera> Initialise(int width, int height, int type);

    virtual ~Camera() = default;
    virtual void ProcessInput(GLFWwindow* window, float deltaTime) = 0;
    virtual void ProcessMouse(double xPos, double yPos) = 0;
    virtual void ProcessMouseButtons(GLFWwindow* window, int button, int action) = 0;
    virtual void UpdateView() = 0;

  public:
    glm::mat4 view {};
    glm::mat4 projection { glm::mat4(1.0f) };
    CameraAngles angles {};
    MousePosition mousepos {};

  private:
  };
}