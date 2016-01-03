#include "shader.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <GL/glew.h>

namespace render {

VertexShader::VertexShader(const std::string& sourceCode)
    : Shader(GL_VERTEX_SHADER, sourceCode)
{
}

FragmentShader::FragmentShader(const std::string& sourceCode)
    : Shader(GL_FRAGMENT_SHADER, sourceCode)
{
}
  
Shader::Shader(GLenum shaderType, const std::string& sourceCode)
    : id_(glCreateShader(shaderType)) 
{
  auto shaderCodeCPtr = sourceCode.c_str();
  glShaderSource(id_, 1, &shaderCodeCPtr, nullptr);
  glCompileShader(id_);

  // Check Vertex Shader
  GLint result;
  glGetShaderiv(id_, GL_COMPILE_STATUS, &result);
  if (result != GL_TRUE) {
    GLint compileLogLength;
    glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &compileLogLength);
    std::string compileLog(compileLogLength + 1, ' ');
    if ( compileLogLength > 0 ) {
      glGetShaderInfoLog(id_, compileLogLength, nullptr, &compileLog[0]);
      glDeleteShader(id_);
      throw std::runtime_error("Shader compilation error: " + compileLog);
    }
  }
}

Shader::~Shader() {
  glDeleteShader(id_);
}

GLuint Shader::getID() {
  return id_;
}



}
