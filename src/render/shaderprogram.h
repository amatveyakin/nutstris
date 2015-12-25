#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#include <GL/glew.h>

namespace render {
class VertexShader;
class FragmentShader;

class ShaderProgram {
public:
  ShaderProgram(VertexShader& vertexShader, FragmentShader& fragmentShader);
  ~ShaderProgram();

  void makeActive();

private:
  GLuint id_;
  GLuint VPmatrixID_;
  double t = 0.0;
};

}
#endif // SHADERPROGRAM_H
