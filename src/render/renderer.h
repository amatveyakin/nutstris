#ifndef RENDERER_H
#define RENDERER_H
#include <memory>
#include <GL/glew.h>

namespace render {
class CubeMesh;

class Renderer {
public:
  Renderer();
  ~Renderer();
  
  void render();
private:
  std::unique_ptr<CubeMesh> cubeMesh_;
};

}

#endif // RENDERER_H
