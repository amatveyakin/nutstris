#ifndef CUBEMESH_H
#define CUBEMESH_H
#include <memory>
#include <GL/glew.h>

namespace render {
class ShaderProgram;

class CubeMesh {
public:
  CubeMesh();
  ~CubeMesh();
  
  void render();
  
private:
  GLuint vertexArrayID_;
  std::unique_ptr<ShaderProgram> shaderProgram_;
};

}
#endif // CUBEMESH_H
