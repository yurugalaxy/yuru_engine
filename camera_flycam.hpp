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

  /*BUG: I don't know why it won't work wiht a virtual interface base class
   *    I just keep getting undefined references to the virtual destructor,
   *    even though I implement one as default, what the heck?
   */

  // class Camera
  // {
  // public:
  //   virtual ~Camera() = default;
  //   virtual void processInput(GLFWwindow* window, float deltaTime);
  //   virtual void processMouse(double xPos, double yPos);
  //
  // public:
  //   CameraAngles angles {};
  //   MousePosition mousepos {};
  // };

  class ProjectionCam //: public Camera
  {
  public:
    ProjectionCam(float screenWidth, float screenHeight, float fov);
    ~ProjectionCam();
    void processInput(GLFWwindow* window, float deltaTime);
    void processMouse(double xPos, double yPos);
    void processMouseButtons(GLFWwindow* window, int button, int action, int mods);

  public:
    CameraAngles angles {};
    MousePosition mousepos {};
    float FOV {};
  };
}
