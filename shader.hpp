//
// Created by kae on 09/08/24.
//

#pragma once

namespace Yuru
{
  class Shader
  {
  public:
    virtual ~Shader() = default;
    virtual void use() const = 0;

    static Shader* create(const char* vertexPath, const char* fragmentPath);
  };
}