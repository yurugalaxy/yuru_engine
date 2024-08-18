#include "camera_2d.hpp"
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace Lyonesse
{
  Camera2D::Camera2D(float width, float height)
  {
    projection = glm::ortho(0.0f, width, 0.0f, height, 0.1f, 100.0f);
  }

  void Camera2D::ProcessInput(GLFWwindow* window, float deltaTime)
  {
  }

  void Camera2D::ProcessMouse(double xPos, double yPos)
  {
  }

  void Camera2D::ProcessMouseButtons(GLFWwindow* window, int button, int action)
  {
  }

  void Camera2D::UpdateView()
  {
  }
}
