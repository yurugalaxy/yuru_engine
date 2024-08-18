//
// Created by kae on 12/08/24.
//

#pragma once

#include <glm/glm.hpp>

#include "shader.hpp"

namespace Lyonesse
{
  class OpenGLShader : public Shader
  {
  public:
    OpenGLShader(const char* vertexPath, const char* fragmentPath);
    ~OpenGLShader() override;
    void Use() const override;
    void UploadUniformBool(const std::string& name, bool value) const;
    void UploadUniformInt(const std::string& name, int value) const;
    void UploadUniformFloat(const std::string& name, float value) const;
    void UploadUniformFloat2(const std::string& name, const glm::vec2& value) const;
    void UploadUniformFloat3(const std::string& name, const glm::vec3& value) const;
    void UploadUniformFloat4(const std::string& name, const glm::vec4& value) const;
    void UploadUniformMat3(const std::string& name, const glm::mat3& value) const;
    void UploadUniformMat4(const std::string& name, const glm::mat4& value) const;

  public:
    uint32_t m_shaderID {};
  };
}
