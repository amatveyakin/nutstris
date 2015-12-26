#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#include <string>
#include <GL/glew.h>
#include "math/matrix.h"


namespace render {
class VertexShader;
class FragmentShader;

class ShaderProgram {
public:
  ShaderProgram(VertexShader& vertexShader, FragmentShader& fragmentShader);
  ~ShaderProgram();

  void setMatrix(const std::string& name, const math::Mat4x4f& matrix);

  void makeActive();

private:
  GLuint id_;
};

}
#endif // SHADERPROGRAM_H
