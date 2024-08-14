//
// Created by kae on 09/08/24.
//

#pragma once

#include "base.hpp"

namespace Yuru
{
  class OpenGLShader;

  class Shader
  {
  public:
    static Shared<OpenGLShader> Create(const char* vertexPath, const char* fragmentPath);

    virtual ~Shader() = default;
    virtual void Use() const = 0;
  };
}