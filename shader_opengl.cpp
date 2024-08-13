#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "shader_opengl.hpp"

namespace Yuru
{
  OpenGLShader::OpenGLShader(const char* vertexPath, const char* fragmentPath)
  {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
      vShaderFile.open(vertexPath);
      fShaderFile.open(fragmentPath);
      std::stringstream vShaderStream, fShaderStream;

      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();

      vShaderFile.close();
      fShaderFile.close();

      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure& e)
    {
      std::cout << "Shader file not read.\n";
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
      std::cout << "Vertex shader compilation failed." << infoLog << '\n';
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
      std::cout << "Fragment shader compilation failed." << infoLog << '\n';
    }

    m_shaderID = glCreateProgram();
    glAttachShader(m_shaderID, vertex);
    glAttachShader(m_shaderID, fragment);
    glLinkProgram(m_shaderID);

    glGetProgramiv(m_shaderID, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(m_shaderID, 512, nullptr, infoLog);
      std::cout << "Shader program linking failed." << infoLog << '\n';
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    std::cout << "OpenGL Shader " << m_shaderID << " successfully created.\n";
  }

  OpenGLShader::~OpenGLShader()
  {
    std::cout << "OpenGL Shader " << m_shaderID << " destroyed.\n";
    glDeleteProgram(m_shaderID);
  }


  void OpenGLShader::use() const
  {
    glUseProgram(m_shaderID);
  }

  void OpenGLShader::uniformBool(const std::string& name, const bool value) const
  {
    const GLint location = glGetUniformLocation(m_shaderID, name.c_str());
    glUniform1i(location, value);
  }

  void OpenGLShader::uniformInt(const std::string& name, const int value) const
  {
    const GLint location = glGetUniformLocation(m_shaderID, name.c_str());
    glUniform1i(location, value);
  }

  void OpenGLShader::uniformFloat(const std::string& name, const float value) const
  {
    const GLint location = glGetUniformLocation(m_shaderID, name.c_str());
    glUniform1f(location, value);
  }

  void OpenGLShader::uniformFloat2(const std::string& name, glm::vec2& value) const
  {
    const GLint location = glGetUniformLocation(m_shaderID, name.c_str());
    glUniform2f(location, value.x, value.y);
  }

  void OpenGLShader::uniformFloat3(const std::string& name, glm::vec3& value) const
  {
    const GLint location = glGetUniformLocation(m_shaderID, name.c_str());
    glUniform3f(location, value.x, value.y, value.z);
  }

  void OpenGLShader::uniformFloat4(const std::string& name, glm::vec4& value) const
  {
    const GLint location = glGetUniformLocation(m_shaderID, name.c_str());
    glUniform4f(location, value.x, value.y, value.z, value.w);
  }

  void OpenGLShader::uniformMat3(const std::string& name, const glm::mat3& value) const
  {
    const GLint location = glGetUniformLocation(m_shaderID, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
  }

  void OpenGLShader::uniformMat4(const std::string& name, const glm::mat4& value) const
  {
    const GLint location = glGetUniformLocation(m_shaderID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0] );
  }
}
