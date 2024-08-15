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
  GLfloat cube[108]
  {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
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

  /******************************
   * Texture loading
   ******************************/

  glActiveTexture(GL_TEXTURE0);
  Yuru::Texture baseTexture;
  baseTexture.Load("../resources/container.jpg");

  Cube testCube;

  /******************************
   * Vertex and fragment loading
   ******************************/

  Yuru::Shared<Yuru::OpenGLShader> defaultShader = Yuru::Shader::Create("../shaders/vertShader.vert"
                                                                        , "../shaders/fragShader.frag");
  defaultShader->Use();
  defaultShader->UploadUniformFloat("playerTex", 1);

  //     // VBO - raw vertices  // VAO - holds VBOs   //EBO - for indices
  GLuint VBOcube, VBOtex, vertexArrayObjectVAO;

  //Generate each buffer, &reference if only 1, else array
  glGenVertexArrays(1, &vertexArrayObjectVAO);
  glGenBuffers(1, &VBOcube);
  glGenBuffers(1, &VBOtex);

  // Bind the VAO for use with all subsequent VBOs
  glBindVertexArray(vertexArrayObjectVAO);

  // Copy vertices into the VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBOcube);
  glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(GLfloat), testCube.cube, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, VBOtex);
  glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(GLfloat), testCube.textures, GL_STATIC_DRAW);

  // Enable the right array (used in "layout location = X" in the vertex shader)
  // Set stride and index of VBO, if data is tightly packed we can use 0 for the stride
  glBindBuffer(GL_ARRAY_BUFFER, VBOcube);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, VBOtex);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
  glEnableVertexAttribArray(1);

  /******************************
   * Game loop
   ******************************/

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // hewo.SetPosition(hewo.position.posX, hewo.position.posY, 16, 192, 176);
    //Clear the framebuffer
    glClearColor(0.8f, 0.5f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //Calc delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    //Matrices
    auto model = glm::mat4(1.0f);
    auto projection = glm::mat4(1.0f);

    glm::mat4 view = glm::lookAt(beeCam.angles.cameraPos, beeCam.angles.cameraPos + beeCam.angles.cameraFront, beeCam.angles.cameraUp);
    defaultShader->UploadUniformMat4("view", view);

    projection = glm::perspective(glm::radians(beeCam.FOV), 640.0f / 480.0f,0.1f, 100.0f);
    defaultShader->UploadUniformMat4("projection", projection);

    model = glm::mat4(1.0f);
    model = glm::translate(model, testCube.offsetMod);
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.5f, 0.0f));
    model = glm::scale(model, testCube.sizeMod);
    defaultShader->UploadUniformMat4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    //ImGui stuff
    ImGui::Begin("awoo");
    ImGui::Text("testing");
    ImGui::SliderFloat3("Position", &testCube.offsetMod.x, -10.0f, 10.0f);
    ImGui::SliderFloat3("Size", &testCube.sizeMod.x, 0.0f, 5.0f);
    // ImGui::SliderInt2("Sprite", &hewo.position.posX, 0, 12);
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
