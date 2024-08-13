//
// Created by kae on 12/08/24.
//

#pragma once

#include <glm/glm.hpp>
#include <string>

#include "shader.hpp"

namespace Yuru
{
  class OpenGLShader : public Shader
  {
  public:
    OpenGLShader(const char* vertexPath, const char* fragmentPath);
    virtual ~OpenGLShader();
    void use() const override;
    void uniformBool(const std::string& name, bool value) const;
    void uniformInt(const std::string& name, int value) const;
    void uniformFloat(const std::string& name, float value) const;
    void uniformFloat2(const std::string& name, glm::vec2& value) const;
    void uniformFloat3(const std::string& name, glm::vec3& value) const;
    void uniformFloat4(const std::string& name, glm::vec4& value) const;
    void uniformMat3(const std::string& name, const glm::mat3 &value) const;
    void uniformMat4(const std::string& name, const glm::mat4 &value) const;

  private:
    uint32_t m_shaderID {};
  };
}
