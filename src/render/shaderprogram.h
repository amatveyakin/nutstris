#pragma once

#include <string>
#include <GL/glew.h>

#include "math/matrix.h"
#include "math/vector.h"
#include "render/glhandle.h"


namespace render {
class VertexShader;
class FragmentShader;

class ShaderProgram {
public:
  ShaderProgram(VertexShader& vertexShader, FragmentShader& fragmentShader);
  ~ShaderProgram();

  void setUniform(const std::string& name, const math::Mat4x4f& value);
  void setUniform(const std::string& name, const math::Vec4f& value);
  void setUniform(const std::string& name, int value);
  void setUniform(const std::string& name, float value);

  void makeActive();

private:
  GLHandle id_;
};

}
