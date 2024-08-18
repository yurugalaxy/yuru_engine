//
// Created by kae on 18/08/24.
//

#pragma once

#include "camera.hpp"

namespace Lyonesse
{
  class Camera2D : public Camera
  {
  public:
    Camera2D(float width, float height);
    ~Camera2D() override;

    void ProcessInput(GLFWwindow* window, float deltaTime) override;
    void ProcessMouse(double xPos, double yPos) override;
    void ProcessMouseButtons(GLFWwindow* window, int button, int action) override;
    void UpdateView() override;

  private:
  };
}
