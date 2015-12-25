#include "shaderprogram.h"
#include <stdexcept>
#include "render/shader.h"
#include "render/matrixutil.h"

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

  MVPmatrixID_ = glGetUniformLocation(id_, "MVP");
  modelToWorldMatrixID_ = glGetUniformLocation(id_, "ModelToWorld");
}

ShaderProgram::~ShaderProgram() {
  glDeleteProgram(id_);
}

void ShaderProgram::makeActive() {
  glUseProgram(id_);
  auto matrixWorld = matrixutil::rotation({0.1, 0.2, 0.3}, t);
  t += 0.01;
  auto matrixView = matrixutil::lookAt({4.5, 4.5, 4.5}, {0, 0, 0}, {0, 1, 0});
  auto matrixProj = /*math::Mat4x4f::identityMatrix();*/  matrixutil::perspective(M_PI / 2.0, 4.0 / 3.0, 0.01f, 100.0f);
  auto MVP = matrixProj * matrixView * matrixWorld;
  glUniformMatrix4fv(MVPmatrixID_, 1, GL_FALSE, MVP.elements());
  glUniformMatrix4fv(modelToWorldMatrixID_, 1, GL_FALSE, matrixWorld.elements());
}


}