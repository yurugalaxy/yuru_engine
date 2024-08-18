#include "camera.hpp"

#include <memory>

#include "camera_2d.hpp"
#include "camera_creative.hpp"
#include "renderer.hpp"

namespace Lyonesse
{
  Shared<Camera> Camera::Initialise(const int width, const int height, const int type)
  {
    switch(Renderer::GetAPI())
    {
      case RenderAPI::None:
        return nullptr;
      case RenderAPI::OpenGL:
        switch (type)
        {
          case 0:
            return std::make_shared<Camera2D>(Camera2D(width, height));
          case 1:
            return std::make_shared<CreativeCamera>(CreativeCamera(width, height, 45));
          default:
            return nullptr;
        }
      default:
        return nullptr;
    }
  }
}
