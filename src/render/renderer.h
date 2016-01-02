#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <GL/glew.h>

#include "render/cubemesh.h"
#include "render/playerviewport.h"

namespace render {
class TextureLoader;

class Renderer {
public:
  Renderer();
  ~Renderer();
  
  void updatePlayerViewports(int nPlayers, int screenWidth, int screenHeight);

  void prepareToDrawPlayer(size_t iPlayer);

  void render(const std::vector<CubeMesh::PerCubeData>& cubesData, math::Vec4f clipPlane = {});

private:
  std::unique_ptr<CubeMesh> cubeMesh_;
  std::unique_ptr<TextureLoader> textureLoader_;

  std::vector<PlayerViewport> playerViewports_;
};

}

#endif // RENDERER_H
