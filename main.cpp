#include <iostream>
#include <cmath>
#include <memory>

//Main GL
#include <glad/glad.h>

//Yuru engine stuff
#include "base.hpp"
#include "shader_opengl.hpp"
#include "camera_creative.hpp"
#include "window.hpp"

// Matrices and vector transforms
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

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
Lyonesse::Shared<Lyonesse::Camera> beeCam = Lyonesse::Camera::Initialise(1920, 1080, 1);
// Lyonesse::Shared<Lyonesse::Camera> cam2d = Lyonesse::Camera::Initialise(1920, 1080, 0);

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height)
{
  glViewport(0, 0, width, height);
}

// void mouse_callback(GLFWwindow* window, const double xPos, const double yPos)
// {
//   beeCam->ProcessMouse(xPos, yPos);
// }
//
// void mouse_button_callback(GLFWwindow* window, const int button, const int action, const int mods)
// {
//   beeCam->ProcessMouseButtons(window, button, action);
// }

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

struct Quad
{
  std::vector<GLfloat> vertices
  {
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,

    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f
  };
};

  /******************************
   * Main
   ******************************/

int main()
{
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  GLFWwindow* window = Lyonesse::InitialiseWindow(1920, 1080, "wah");
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  // glfwSetCursorPosCallback(window, mouse_callback);
  // glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();

  //Load glad, all OpenGL function pointers
  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
  {
    std::cout << "Failed to initialise GLAD.\n";
  }

  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  Cube testCube;
  Cube lightCube;

  Lyonesse::Shared<Lyonesse::OpenGLShader> lightShader = Lyonesse::Shader::Initialise("../shaders/lightShader.vert"
                                                                , "../shaders/lightShader.frag");

  Lyonesse::Shared<Lyonesse::OpenGLShader> lightSourceShader = Lyonesse::Shader::Initialise("../shaders/lightShader.vert"
                                                                            , "../shaders/lightSource.frag");

  //     // VBO - raw vertices  // VAO - holds VBOs   //EBO - for indices
  GLuint cubeVBO, normalsVBO, textureVBO, lightVBO, cubeVAO, lightVAO;

  //Generate each buffer, &reference if only 1, else array
  glGenVertexArrays(1, &cubeVAO);
  glGenVertexArrays(1, &lightVAO);
  glGenBuffers(1, &cubeVBO);
  glGenBuffers(1, &normalsVBO);
  glGenBuffers(1, &textureVBO);
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

  glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
  glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(GLfloat), testCube.textures, GL_STATIC_DRAW);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
  glEnableVertexAttribArray(2);

  glBindVertexArray(lightVAO);

  glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
  glBufferData(GL_ARRAY_BUFFER, 216 * sizeof(GLfloat), lightCube.cube, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
  glEnableVertexAttribArray(0);

  lightCube.offsetMod = glm::vec3(0.9f, 1.5f, 0.8f);
  lightCube.sizeMod = glm::vec3(0.2f);
  glm::vec3 lightColour { 1.0f, 1.0f, 1.0f };
  float shininess { 32.0f };
  glm::vec3 ambientLight { 0.2f, 0.2f, 0.2f };
  glm::vec3 diffuseLight { 0.5f, 0.5f, 0.5f };
  glm::vec3 specularLight { 1.0f, 1.0f, 1.0f };

  Lyonesse::Texture::Generate("../resources/container2.png", 0);
  Lyonesse::Texture::Generate("../resources/container2_specular.png", 1);

  lightShader->Use();
  lightShader->UploadUniformInt("material.diffuse", 0);
  lightShader->UploadUniformInt("material.specular", 1);

  /******************************
   * Game loop
   ******************************/

  while (!glfwWindowShouldClose(window))
  {
    //Calc delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);
    // beeCam->ProcessInput(window, deltaTime);

    //Clear the framebuffer
    glClearColor(0.15f, 0.12f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //Matrices
    lightShader->Use();
    lightShader->UploadUniformFloat3("lightPos", lightCube.offsetMod);
    // lightShader->UploadUniformFloat3("viewPos", beeCam->angles.cameraPos);

    lightShader->UploadUniformFloat3("light.ambient", ambientLight);
    lightShader->UploadUniformFloat3("light.diffuse", diffuseLight);
    lightShader->UploadUniformFloat3("light.specular", specularLight);
    lightShader->UploadUniformFloat3("lightColour", lightColour);

    lightShader->UploadUniformFloat("material.shininess", shininess);

    lightShader->UploadUniformMat4("projection", beeCam->projection);
    // lightShader->UploadUniformMat4("view", beeCam->view);

    auto model = glm::mat4(1.0f);
    model = translate(model, testCube.offsetMod);
    model = scale(model, testCube.sizeMod);
    lightShader->UploadUniformMat4("model", model);

    auto inverseModel = glm::mat3(transpose(inverse(model)));
    lightShader->UploadUniformMat3("inverseModel", inverseModel);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lightSourceShader->Use();
    // lightSourceShader->UploadUniformMat4("view", beeCam->view);
    lightSourceShader->UploadUniformMat4("projection", beeCam->projection);
    lightSourceShader->UploadUniformFloat3("lightColour", lightColour);

    auto modelLight = glm::mat4(1.0f);
    modelLight = translate(modelLight, lightCube.offsetMod);
    modelLight = scale(modelLight, lightCube.sizeMod);
    lightSourceShader->UploadUniformMat4("model", modelLight);

    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //ImGui stuff
    ImGui::Begin("awoo");
    ImGui::Text("testing");
    ImGui::SliderFloat3("Cube Position", &testCube.offsetMod.x, -10.0f, 10.0f);
    ImGui::SliderFloat3("Cube Size", &testCube.sizeMod.x, 0.0f, 5.0f);
    ImGui::SliderFloat3("Light Position", &lightCube.offsetMod.x, -10.0f, 10.0f);
    ImGui::SliderFloat3("Light Colour", &lightColour.x, 0.0f, 1.0f);
    ImGui::SliderFloat("Shininess", &shininess, 0.0f, 256.0f);
    ImGui::SliderFloat3("Ambient Light", &ambientLight.x, 0.0f, 1.0f);
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
