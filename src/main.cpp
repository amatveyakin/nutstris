#include <chrono>

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

#include "render/renderer.h"
#include "engine/engine.h"


math::Vec3f ColorToVec3(engine::Color c) {
  return {c.r / 255.0f, c.g / 255.0f, c.b / 255.0f};
}

// Stub.  TODO(Alexey): implement actual rendering.
void drawGame(render::Renderer& renderer, engine::Game& game, engine::Time now) {
  for (auto* player : game.participants) {
    if (player->active) {
      std::vector<render::CubeMesh::PerCubeData> cubesData;
      auto addBlocks = [&cubesData, now](std::vector<engine::BlockImage>& blockImages) {
        for (auto& lyingBlock : blockImages) {
          auto pos2d = lyingBlock.position(now);
          cubesData.push_back({math::Mat4x4f::translationMatrix({pos2d.x() - engine::FIELD_WIDTH / 2, pos2d.y() - engine::FIELD_HEIGHT / 2, 0.0f}),
                              ColorToVec3(lyingBlock.color), 0});
        }
      };
      addBlocks(player->lyingBlockImages);
      addBlocks(player->fallingBlockImages);
      renderer.render(cubesData);
      return;
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
        glViewport(0, 0, event.size.width, event.size.height);
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
