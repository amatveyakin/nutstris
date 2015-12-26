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

  VPmatrixID_ = glGetUniformLocation(id_, "VP");
}

ShaderProgram::~ShaderProgram() {
  glDeleteProgram(id_);
}

void ShaderProgram::makeActive() {
  glUseProgram(id_);
  auto matrixView = matrixutil::lookAt({0.0, 1.5, 10.5}, {0, 0, 0}, {0, 1, 0});

  int viewPortInfo[4];
  glGetIntegerv(GL_VIEWPORT, viewPortInfo);
  float widthToHeightAspect = float(viewPortInfo[2]) / float(viewPortInfo[3]);
  auto matrixProj = /*math::Mat4x4f::identityMatrix();*/  matrixutil::perspective(M_PI / 2.0, widthToHeightAspect, 0.01f, 100.0f);
  auto VP = matrixProj * matrixView;
  glUniformMatrix4fv(VPmatrixID_, 1, GL_FALSE, VP.elements());

  auto texturesID = glGetUniformLocation(id_, "textures");
  glUniform1i(texturesID, 0);
}


}