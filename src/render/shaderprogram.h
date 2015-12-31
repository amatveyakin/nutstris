#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#include <string>
#include <GL/glew.h>
#include "math/matrix.h"
#include "math/vector.h"


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

  void makeActive();

private:
  GLuint id_;
};

}
#endif // SHADERPROGRAM_H
