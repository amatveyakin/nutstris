#ifndef RENDERER_H
#define RENDERER_H
#include <GL/glew.h>

namespace render {

class Renderer {
public:
  Renderer();
  ~Renderer();
  
  void render();
private:
  GLuint vertexArrayID_;
};

}

#endif // RENDERER_H
