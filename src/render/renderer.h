#ifndef RENDERER_H
#define RENDERER_H
#include <memory>
#include <GL/glew.h>

namespace render {
class CubeMesh;
class TextureLoader;

class Renderer {
public:
  Renderer();
  ~Renderer();
  
  void render();
private:
  std::unique_ptr<CubeMesh> cubeMesh_;
  std::unique_ptr<TextureLoader> textureLoader_;
};

}

#endif // RENDERER_H
