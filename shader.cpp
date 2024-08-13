// #include "shader.hpp"
// #include "shader_opengl.hpp"

// #include "renderer.hpp"

// namespace Yuru
// {
//   Shader* Shader::create(const char* vertexPath, const char* fragmentPath)
//   {
//     switch(Renderer::getAPI())
//     {
//       case RenderAPI::None:
//         return nullptr;
//       case RenderAPI::OpenGL:
//         return new OpenGLShader(vertexPath, fragmentPath);
//     }
//       return nullptr;
//   }

// }
