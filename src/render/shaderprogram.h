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
};

}
#endif // SHADERPROGRAM_H
