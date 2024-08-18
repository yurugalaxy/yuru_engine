//
// Created by kae on 13/08/24.
//

#pragma once

#include <GLFW/glfw3.h>

#include "camera.hpp"

namespace Lyonesse
{

  class CreativeCamera : public Camera
  {
  public:
    CreativeCamera(float screenWidth, float screenHeight, float fov);
    ~CreativeCamera() override;
    void ProcessInput(GLFWwindow* window, float deltaTime) override;
    void ProcessMouse(double xPos, double yPos) override;
    void ProcessMouseButtons(GLFWwindow* window, int button, int action) override;
    void UpdateView() override;

  public:
    float FOV {};

  private:
  };
}
