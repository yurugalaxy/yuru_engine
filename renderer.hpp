//
// Created by kae on 12/08/24.
//

#pragma once

namespace Yuru
{
  enum class RenderAPI
  {
    None = 0,
    OpenGL = 1
  };

  class Renderer
  {
  public:
    static RenderAPI getAPI(){ return s_API; };

  public:
    static RenderAPI s_API;
  };
}