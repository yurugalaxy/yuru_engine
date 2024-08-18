#include "shader.hpp"

#include <memory>
#include "shader_opengl.hpp"
#include "renderer.hpp"

namespace Lyonesse
{
  Shared<OpenGLShader> Shader::Initialise(const char* vertexPath, const char* fragmentPath)
  {
    switch(Renderer::GetAPI())
    {
      case RenderAPI::None:
        return nullptr;
      case RenderAPI::OpenGL:
        return std::make_shared<OpenGLShader>(vertexPath, fragmentPath);
    }
      return nullptr;
  }

}
