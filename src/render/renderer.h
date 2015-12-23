#ifndef RENDERER_H
#define RENDERER_H
#include <memory>
#include <GL/glew.h>

namespace render {
class ShaderProgram;

class Renderer {
public:
  Renderer();
  ~Renderer();
  
  void render();
private:
  GLuint vertexArrayID_;

  std::unique_ptr<ShaderProgram> dummyProgram_;
};

}

#endif // RENDERER_H
