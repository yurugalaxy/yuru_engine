#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera_creative.hpp"

namespace Yuru
{
  CreativeCamera::CreativeCamera(const float screenWidth, const float screenHeight, float fov)
  {
    angles.cameraPos.z = 3.0f;
    angles.cameraFront.z = -1.0f;
    angles.cameraUp.y = 1.0f;
    angles.yaw = -90.0f;
    FOV = fov;
    mousepos.mouseX = screenWidth / 2.0f;
    mousepos.mouseY = screenHeight / 2.0f;
  }

  CreativeCamera::~CreativeCamera()
  {
    std::cout << "Destroying projection cam.\n";
  }

  void CreativeCamera::ProcessInput(GLFWwindow* window, const float deltaTime)
  {
    const float cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      angles.cameraPos += cameraSpeed * angles.cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      angles.cameraPos -= cameraSpeed * angles.cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      angles.cameraPos -= cameraSpeed * glm::normalize(glm::cross(angles.cameraFront, angles.cameraUp));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      angles.cameraPos += cameraSpeed * glm::normalize(glm::cross(angles.cameraFront, angles.cameraUp));
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      angles.cameraPos.y += cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
      angles.cameraPos.y -= cameraSpeed;
  }

  void CreativeCamera::ProcessMouse(const double xPos, const double yPos)
  {
    if (!mousepos.mouseActive)
    {
      mousepos.firstMouseMove = true;
      return;
    }

    if (mousepos.firstMouseMove)
    {
      mousepos.mouseX = 0.0f;
      mousepos.mouseY = 0.0f;
      mousepos.firstMouseMove = false;
    }

    float xOffset = xPos - mousepos.mouseX;
    float yOffset = mousepos.mouseY - yPos; // Reversed because Y is from bottom to top
    mousepos.mouseX = xPos;
    mousepos.mouseY = yPos;

    constexpr float sensitivity = 0.1f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    angles.yaw += xOffset;
    angles.pitch += yOffset;

    if (angles.pitch > 89.0f)
      angles.pitch = 89.0f;
    if (angles.pitch < -89.0f)
      angles.pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(angles.yaw)) * cos(glm::radians(angles.pitch));
    front.y = sin(glm::radians(angles.pitch));
    front.z = sin(glm::radians(angles.yaw)) * cos(glm::radians(angles.pitch));
    angles.cameraFront = glm::normalize(front);
  }

  void CreativeCamera::ProcessMouseButtons(GLFWwindow* window, int button, int action, int mods)
  {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
      mousepos.mouseActive = true;
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      glfwSetCursorPos(window, 0.0, 0.0);
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
      mousepos.mouseActive = false;
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  }

  glm::mat4 CreativeCamera::GetViewMatrix() const
  {
    return lookAt(angles.cameraPos, angles.cameraPos + angles.cameraFront, angles.cameraUp);
  }
}
