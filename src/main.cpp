#include <chrono>

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

#include "render/renderer.h"
#include "render/defs.h"
#include "render/matrixutil.h"
#include "engine/engine.h"

math::Vec3f ColorToVec3(engine::Color c) {
  return {c.r / 255.0f, c.g / 255.0f, c.b / 255.0f};
}

float fieldToWorldX(float fieldX) {
  return (fieldX - (engine::FIELD_WIDTH - 1.0f) / 2.0f) * render::CUBE_SCALE;
}

float fieldToWorldY(float fieldY) {
  return (fieldY - (engine::FIELD_HEIGHT - 1.0f) / 2.0f) * render::CUBE_SCALE;
}

math::Mat4x4f fieldPosToWorldPos(int fieldX, int fieldY) {
  return math::Mat4x4f::translationMatrix({fieldToWorldX(fieldX), fieldToWorldY(fieldY), 0.0f}) *
         render::matrixutil::scale(render::CUBE_SCALE);
}

// Stub.  TODO(Alexey): implement actual rendering.
void drawGame(render::Renderer& renderer, engine::Game& game, engine::Time now) {
  for (size_t iPlayer = 0; iPlayer < game.participants.size(); ++iPlayer) {
    auto* player = game.participants[iPlayer];
    if (player->active) {
      renderer.prepareToDrawPlayer(iPlayer);
      std::vector<render::CubeMesh::PerCubeData> cubesData;
      auto addBlocks = [&cubesData, now](std::vector<engine::BlockImage>& blockImages) {
        for (auto& lyingBlock : blockImages) {
          auto pos2d = lyingBlock.position(now);
          cubesData.push_back({fieldPosToWorldPos(pos2d.x(), pos2d.y()), ColorToVec3(lyingBlock.color), 0});
        }
      };
      addBlocks(player->lyingBlockImages);
      addBlocks(player->fallingBlockImages);
      renderer.render(cubesData);

      for ( size_t iDisappearingLine = 0; iDisappearingLine < player->disappearingLines.size(); ++iDisappearingLine ) {
        auto& currentLine = player->disappearingLines[iDisappearingLine];
        math::Vec4f clippingPlane = { 2.0f * ( iDisappearingLine % 2 ) - 1.0f, 1.0f, 1.0f,
                                      1.5f * ( 2.f * currentLine.progress ( now ) - 1.f)};
        std::vector<render::CubeMesh::PerCubeData> lineCubesData;
        for (size_t x = 0; x < engine::FIELD_WIDTH; ++x)
          lineCubesData.push_back({fieldPosToWorldPos(x, currentLine.row), ColorToVec3(currentLine.blockColor[x]), 0});
        renderer.render(lineCubesData, clippingPlane);
      }
    }
  }
}

int main() {
  sf::ContextSettings contextSettings;
  contextSettings.depthBits = 24;
  contextSettings.antialiasingLevel = 16;
  sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, contextSettings);
  window.setVerticalSyncEnabled(true);

  engine::Game game;
  render::Renderer renderer;

  game.init();
  game.newMatch();
  game.newRound(std::chrono::steady_clock::now().time_since_epoch());

  bool running = true;
  while (running) {
    // handle events
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        running = false;
      }
      else if (event.type == sf::Event::Resized) {
        renderer.updatePlayerViewports(game.participants.size(), event.size.width, event.size.height);
      }
    }

    // update game
    auto now = std::chrono::steady_clock::now().time_since_epoch();
    game.onTimer(now);

    // draw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawGame(renderer, game, now);
    window.display();  // swap the front and back buffers
  }

  game.saveSettings();

  return 0;
}
