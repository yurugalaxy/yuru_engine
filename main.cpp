#include <iostream>
#include <cmath>
#include <memory>

//Main GL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Matrices and vector transforms
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// For GL image loading
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

//OpenGL Shader loading
#include "shader_opengl.hpp"

#include "camera_flycam.hpp"

//Window globals
constexpr unsigned int Screen_Width { 640 };
constexpr unsigned int Screen_Height { 480 };

float lastX = Screen_Width / 2.0f;
float lastY = Screen_Height / 2.0f;
float FOV = 45.0f;

//Deltatime globals
float deltaTime = 0.0f;
float lastFrame = 0.0f;

Yuru::ProjectionCam beeCam {Screen_Width, Screen_Height, FOV};

void processInput(GLFWwindow* window)
{
  const float cameraSpeed = 2.5f * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  beeCam.processInput(window, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, const double xPos, const double yPos)
{
  beeCam.processMouse(xPos, yPos);
}

void mouse_button_callback(GLFWwindow* window, const int button, const int action, const int mods)
{
  beeCam.processMouseButtons(window, button, action, mods);
}

//TODO: abstract the texture class
struct SpritePosition
{
  float leftX;
  float rightX;
  float bottomY;
  float topY;
};

SpritePosition loadSprite(const float x, const float y, const float spriteSize, const float sheetWidth, const float sheetHeight)
{
  return {x * spriteSize / sheetWidth
            , (x + 1) * spriteSize / sheetWidth
            ,y * spriteSize / sheetHeight
            , (y + 1) * spriteSize / sheetHeight};
}

void loadSpriteSheet(const char* path)
{
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  //Texure wrapping & filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data = stbi_load(path
                                  , &width
                                  , &height
                                  , &nrChannels
                                  , 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D
                , 0
                ,GL_RGBA
                , width
                , height
                , 0
                , GL_RGBA
                , GL_UNSIGNED_BYTE
                , data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture.\n";
  }
  stbi_image_free(data);
}

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
  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
  loadSpriteSheet("../resources/tilemap_packed.png");
  SpritePosition grass = loadSprite(7, 7, 16.0f, 192.0f, 176.0f);
  SpritePosition dirt = loadSprite(3, 7, 16.0f, 192.0f, 176.0f);

  /******************************
   * Vertex and fragment loading
   ******************************/

  Yuru::OpenGLShader defaultShader("../shaders/vertShader.vert","../shaders/fragShader.frag");
  defaultShader.use();

  GLfloat cube[]
  {
    -0.5f, -0.5f, -0.5f,  grass.leftX,  grass.bottomY,
     0.5f, -0.5f, -0.5f,  grass.rightX, grass.bottomY,
     0.5f,  0.5f, -0.5f,  grass.rightX, grass.topY,
     0.5f,  0.5f, -0.5f,  grass.rightX, grass.topY,
    -0.5f,  0.5f, -0.5f,  grass.leftX,  grass.topY,
    -0.5f, -0.5f, -0.5f,  grass.leftX,  grass.bottomY,

    -0.5f, -0.5f,  0.5f,  dirt.leftX,  dirt.bottomY,
     0.5f, -0.5f,  0.5f,  dirt.rightX, dirt.bottomY,
     0.5f,  0.5f,  0.5f,  dirt.rightX, dirt.topY,
     0.5f,  0.5f,  0.5f,  dirt.rightX, dirt.topY,
    -0.5f,  0.5f,  0.5f,  dirt.leftX,  dirt.topY,
    -0.5f, -0.5f,  0.5f,  dirt.leftX,  dirt.bottomY,

    -0.5f,  0.5f,  0.5f,  grass.rightX, grass.bottomY,
    -0.5f,  0.5f, -0.5f,  grass.rightX, grass.topY,
    -0.5f, -0.5f, -0.5f,  grass.leftX,  grass.topY,
    -0.5f, -0.5f, -0.5f,  grass.leftX,  grass.topY,
    -0.5f, -0.5f,  0.5f,  grass.leftX,  grass.bottomY,
    -0.5f,  0.5f,  0.5f,  grass.rightX, grass.bottomY,

     0.5f,  0.5f,  0.5f,  grass.rightX, grass.bottomY,
     0.5f,  0.5f, -0.5f,  grass.rightX, grass.topY,
     0.5f, -0.5f, -0.5f,  grass.leftX,  grass.topY,
     0.5f, -0.5f, -0.5f,  grass.leftX,  grass.topY,
     0.5f, -0.5f,  0.5f,  grass.leftX,  grass.bottomY,
     0.5f,  0.5f,  0.5f,  grass.rightX, grass.bottomY,

    -0.5f, -0.5f, -0.5f,  grass.leftX,  grass.topY,
     0.5f, -0.5f, -0.5f,  grass.rightX, grass.topY,
     0.5f, -0.5f,  0.5f,  grass.rightX, grass.bottomY,
     0.5f, -0.5f,  0.5f,  grass.rightX, grass.bottomY,
    -0.5f, -0.5f,  0.5f,  grass.leftX,  grass.bottomY,
    -0.5f, -0.5f, -0.5f,  grass.leftX,  grass.topY,

    -0.5f,  0.5f, -0.5f,  grass.leftX,  grass.topY,
     0.5f,  0.5f, -0.5f,  grass.rightX, grass.topY,
     0.5f,  0.5f,  0.5f,  grass.rightX, grass.bottomY,
     0.5f,  0.5f,  0.5f,  grass.rightX, grass.bottomY,
    -0.5f,  0.5f,  0.5f,  grass.leftX,  grass.bottomY,
    -0.5f,  0.5f, -0.5f,  grass.leftX,  grass.topY
  };

  glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

  //Specify the way to draw each index
  unsigned int indices[] = {
    0, 2, 1,
    1, 3, 2
  };

  defaultShader.uniformFloat("playerTex", 1);

  //     // VBO - for triangles // VAO - holds VBOs   //EBO - for multiple triangles that share vertices
  GLuint vertexBufferObjectVBO, vertexArrayObjectVAO, elementBufferObjectEBO;

  //Generate each buffer, &reference if only 1, else array
  glGenVertexArrays(1, &vertexArrayObjectVAO);
  glGenBuffers(1, &vertexBufferObjectVBO);

  // Bind the VAO for use with all subsequent VBOs
  glBindVertexArray(vertexArrayObjectVAO);

  // Copy vertices into the VBO
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

  // Enable the right array (used in "layout location = X" in the vertex shader)
  glEnableVertexAttribArray(0);
  // Set stride and index of VBO, if data is tightly packed we can use 0 for the stride
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));

  /******************************
   * Game loop
   ******************************/

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    //Clear the framebuffer
    glClearColor(0.8f, 0.5f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    //Matrices
    auto model = glm::mat4(1.0f);
    auto projection = glm::mat4(1.0f);

    float currTime = glfwGetTime();

    glm::mat4 view = glm::lookAt(beeCam.angles.cameraPos, beeCam.angles.cameraPos + beeCam.angles.cameraFront, beeCam.angles.cameraUp);
    defaultShader.uniformMat4("view", view);

    projection = glm::perspective(glm::radians(beeCam.FOV), 640.0f / 480.0f,0.1f, 100.0f);
    // Ortho projection needs to be really small since all objects are in -1.0 - 1.0 space
    // We should also really disable the z buffer to avoid z fighting when using ortho
    // projection = glm::ortho(-2.0f, 2.0f, 2.0f, -2.0f, -1000.0f, 1000.0f);
    defaultShader.uniformMat4("projection", projection);

    // Draw cubes
    for (unsigned int i { 0 }; i < 10; ++i)
    {
      model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      model = glm::rotate(model, currTime * glm::radians(60.0f) * i + 1, glm::vec3(1.0f, 0.5f, 0.0f));
      defaultShader.uniformMat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
