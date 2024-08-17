#include <iostream>
#include <cmath>
#include <memory>

//Main GL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Yuru engine stuff
#include "base.hpp"
#include "shader_opengl.hpp"
#include "camera_creative.hpp"

// Matrices and vector transforms
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// For GL image loading
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

//Imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "texture.hpp"

//Window globals
constexpr unsigned int Screen_Width { 1920 };
constexpr unsigned int Screen_Height { 1080 };

float FOV = 45.0f;

//Deltatime globals
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//Create the flycam
Yuru::CreativeCamera beeCam {Screen_Width, Screen_Height, FOV};

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  beeCam.ProcessInput(window, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height)
{
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, const double xPos, const double yPos)
{
  beeCam.ProcessMouse(xPos, yPos);
}

void mouse_button_callback(GLFWwindow* window, const int button, const int action, const int mods)
{
  beeCam.ProcessMouseButtons(window, button, action, mods);
}

struct Cube
{
  GLfloat cube[216]
  {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
  };

  GLfloat textures[72]
  {
     0.0f, 0.0f,
     1.0f, 0.0f,
     1.0f, 1.0f,
     1.0f, 1.0f,
     0.0f, 1.0f,
     0.0f, 0.0f,

     0.0f, 0.0f,
     1.0f, 0.0f,
     1.0f, 1.0f,
     1.0f, 1.0f,
     0.0f, 1.0f,
     0.0f, 0.0f,

     1.0f, 0.0f,
     1.0f, 1.0f,
     0.0f, 1.0f,
     0.0f, 1.0f,
     0.0f, 0.0f,
     1.0f, 0.0f,

     1.0f, 0.0f,
     1.0f, 1.0f,
     0.0f, 1.0f,
     0.0f, 1.0f,
     0.0f, 0.0f,
     1.0f, 0.0f,

     0.0f, 1.0f,
     1.0f, 1.0f,
     1.0f, 0.0f,
     1.0f, 0.0f,
     0.0f, 0.0f,
     0.0f, 1.0f,

     0.0f, 1.0f,
     1.0f, 1.0f,
     1.0f, 0.0f,
     1.0f, 0.0f,
     0.0f, 0.0f,
     0.0f, 1.0f
  };

  glm::vec3 offsetMod { {}, {}, {}};
  glm::vec3 sizeMod { 1.0f, 1.0f, 1.0f };
};
  /******************************
   * Main
   ******************************/

int main()
{

  /******************************
   * Initialise GLFW
   ******************************/
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  /******************************
   * Create GLFW window
   ******************************/

  GLFWwindow* window = glfwCreateWindow(Screen_Width, Screen_Height, "title", nullptr, nullptr);
  if (window == nullptr)
  {
    std::cout << "Failed to create GLFW window.\n";
    glfwTerminate();
    return -1;
  }
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();

  //Load glad, all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialise GLAD.\n";
    return -1;
  }

  //Enable the depth buffer
  glEnable(GL_DEPTH_TEST);

  //Default display mode
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  //Wireframe mode
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  Cube testCube;
  Cube lightCube;

  Yuru::Shared<Yuru::OpenGLShader> lightShader = Yuru::Shader::Create("../shaders/lightShader.vert"
                                                                , "../shaders/lightShader.frag");

  Yuru::Shared<Yuru::OpenGLShader> lightSourceShader = Yuru::Shader::Create("../shaders/lightShader.vert"
                                                                            , "../shaders/lightSource.frag");

  //     // VBO - raw vertices  // VAO - holds VBOs   //EBO - for indices
  GLuint cubeVBO, normalsVBO, lightVBO, cubeVAO, lightVAO;

  //Generate each buffer, &reference if only 1, else array
  glGenVertexArrays(1, &cubeVAO);
  glGenVertexArrays(1, &lightVAO);
  glGenBuffers(1, &cubeVBO);
  glGenBuffers(1, &normalsVBO);
  glGenBuffers(1, &lightVBO);

  glBindVertexArray(cubeVAO);

  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, 216 * sizeof(GLfloat), testCube.cube, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
  glBufferData(GL_ARRAY_BUFFER, 216 * sizeof(GLfloat), testCube.cube, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(lightVAO);

  glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
  glBufferData(GL_ARRAY_BUFFER, 216 * sizeof(GLfloat), lightCube.cube, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
  glEnableVertexAttribArray(0);

  lightCube.offsetMod = glm::vec3(0.9f, 1.5f, 0.8f);
  lightCube.sizeMod = glm::vec3(0.2f);
  glm::vec3 objColour { 1.0f, 0.5f, 0.32f };
  glm::vec3 lightColour { 1.0f, 1.0f, 1.0f};

  /******************************
   * Game loop
   ******************************/

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    //Clear the framebuffer
    glClearColor(0.15f, 0.12f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //Calc delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    //Matrices
    auto projection = glm::mat4(1.0f);
    lightShader->Use();
    lightShader->UploadUniformFloat3("objectColour", objColour);
    lightShader->UploadUniformFloat3("lightColour", lightColour);
    lightShader->UploadUniformFloat3("lightPos", lightCube.offsetMod);
    lightShader->UploadUniformFloat3("viewPos", beeCam.angles.cameraPos);

    glm::mat4 view = glm::lookAt(beeCam.angles.cameraPos
                                , beeCam.angles.cameraPos + beeCam.angles.cameraFront
                                , beeCam.angles.cameraUp);

    lightShader->UploadUniformMat4("view", view);

    projection = glm::perspective(glm::radians(beeCam.FOV), 640.0f / 480.0f,0.1f, 100.0f);
    lightShader->UploadUniformMat4("projection", projection);

    auto model = glm::mat4(1.0f);
    model = glm::translate(model, testCube.offsetMod);
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.5f, 0.0f));
    model = glm::scale(model, testCube.sizeMod);
    lightShader->UploadUniformMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lightSourceShader->Use();

    lightSourceShader->UploadUniformMat4("view", view);
    lightSourceShader->UploadUniformMat4("projection", projection);
    lightSourceShader->UploadUniformFloat3("lightColour", lightColour);

    auto modelLight = glm::mat4(1.0f);
    modelLight = glm::translate(modelLight, lightCube.offsetMod);
    modelLight = glm::scale(modelLight, lightCube.sizeMod);
    lightSourceShader->UploadUniformMat4("model", modelLight);

    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //ImGui stuff
    ImGui::Begin("awoo");
    ImGui::Text("testing");
    ImGui::SliderFloat3("Cube Position", &testCube.offsetMod.x, -10.0f, 10.0f);
    ImGui::SliderFloat3("Cube Colour", &objColour.x, 0.0f, 1.0f);
    ImGui::SliderFloat3("Cube Size", &testCube.sizeMod.x, 0.0f, 5.0f);
    ImGui::SliderFloat3("Light Position", &lightCube.offsetMod.x, -10.0f, 10.0f);
    ImGui::SliderFloat3("Light Colour", &lightColour.x, 0.0f, 1.0f);
    ImGui::SliderFloat3("Light Size", &lightCube.sizeMod.x, 0.0f, 5.0f);
    ImGui::End();

    //Required to render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}
