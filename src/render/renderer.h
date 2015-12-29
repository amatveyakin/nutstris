#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <GL/glew.h>

#include "render/cubemesh.h"

namespace render {
class TextureLoader;

class Renderer {
public:
  Renderer();
  ~Renderer();
  
  void render(const std::vector<CubeMesh::PerCubeData>& cubesData);

private:
  std::unique_ptr<CubeMesh> cubeMesh_;
  std::unique_ptr<TextureLoader> textureLoader_;
};

}

#endif // RENDERER_H
