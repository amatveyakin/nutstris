#pragma once

#include <memory>
#include <GL/glew.h>

#include "engine/engine.h"
#include "render/dataformats.h"
#include "render/playerviewport.h"
#include "render/uniformbuffer.h"

namespace render {
class TexturedQuad;
class TextureArray;
class CubeMesh;

class Renderer {
public:
  Renderer();
  ~Renderer();
  
  void renderGame(engine::Game& game, engine::Time now);

  void updatePlayerViewports(int nPlayers, int screenWidth, int screenHeight);

private:
  void renderPlayer_(engine::Player& player, engine::Time now);
  void prepareToDrawPlayer_(size_t iPlayer, engine::Player& player, engine::Time now);
  void renderCubes_(const std::vector<dataformats::CubeInstance>& cubesData, math::Mat4x4f globalRotation, math::Vec4f clipPlane = {});
  void renderDisappearingLines_(const std::vector<engine::DisappearingLine>& lines, math::Mat4x4f globalRotation, engine::Time now);
  void renderWall_(engine::Player& player);

  math::Mat4x4f getViewProjection_() const;
  math::Mat4x4f getGlobalRotation_(engine::Player& player, engine::Time now) const;
  float getWaveProgress(engine::Player & player, engine::Time now) const;

private:
  std::unique_ptr<CubeMesh> cubeMesh_;
  std::unique_ptr<TexturedQuad> wall_;
  std::unique_ptr<TextureArray> bonusesTexture_;
  std::unique_ptr<TextureArray> wallTexture_;

  std::vector<PlayerViewport> playerViewports_;

  std::unique_ptr<UniformBuffer<dataformats::LightsSettings>> lightsSettingsBuffer_;
};

}
