#include "shader.h"
#include <iostream>
#include <fstream>
#include <GL/glew.h>

namespace render {

VertexShader::VertexShader(const std::string& filename) 
    : Shader(GL_VERTEX_SHADER, filename)
{
}

FragmentShader::FragmentShader(const std::string& filename) 
    : Shader(GL_FRAGMENT_SHADER, filename)
{
}
  
Shader::Shader(GLenum shaderType, const std::string& filename)
    : id_(glCreateShader(shaderType)) 
{
  std::ifstream shaderInputStream(filename);
  std::string shaderCode(std::istreambuf_iterator<char>(shaderInputStream), {});

  auto shaderCodeCPtr = shaderCode.c_str();
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
