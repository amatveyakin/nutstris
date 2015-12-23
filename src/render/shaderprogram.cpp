#include "shaderprogram.h"
#include <stdexcept>
#include "render/shader.h"

namespace render {

ShaderProgram::ShaderProgram(VertexShader& vertexShader, FragmentShader& fragmentShader)
    : id_(glCreateProgram())
{
  glAttachShader(id_, vertexShader.getID());
  glAttachShader(id_, fragmentShader.getID());
  glLinkProgram(id_);
  glDetachShader(id_, vertexShader.getID());
  glDetachShader(id_, fragmentShader.getID());

  // Check the program
  GLint result;
  glGetProgramiv(id_, GL_LINK_STATUS, &result);
  if (result != GL_TRUE) {
    GLint linkLogLength;
    glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &linkLogLength);
    std::string linkLog(linkLogLength + 1, ' ');
    if ( linkLogLength > 0 ) {
      glGetProgramInfoLog(id_, linkLogLength, nullptr, &linkLog[0]);
      glDeleteProgram(id_);
      throw std::runtime_error("Shader program link error: " + linkLog);
    }
  }
}

ShaderProgram::~ShaderProgram() {
  glDeleteProgram(id_);
}

void ShaderProgram::makeActive() {
  glUseProgram(id_);
}


}