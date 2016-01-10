#include <chrono>

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

#include "render/renderer.h"
#include "render/defs.h"
#include "render/matrixutil.h"
#include "engine/engine.h"
#include "engine/settings.h"


int main() {
  srand(std::chrono::system_clock::now().time_since_epoch().count());

  sf::ContextSettings contextSettings;
  contextSettings.depthBits = 24;
  contextSettings.antialiasingLevel = 16;
  sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, contextSettings);
  window.setVerticalSyncEnabled(true);

  engine::Game game;
  render::Renderer renderer;

  game.init();
  engine::loadSettings(&game);
  game.newMatch();
  game.newRound(std::chrono::steady_clock::now().time_since_epoch());

  bool running = true;
  renderer.updatePlayerViewports(game.participants.size(), window.getSize().x, window.getSize().y);
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
    renderer.renderGame(game, now);
    window.display();  // swap the front and back buffers
  }

  engine::saveSettings(&game);

  return 0;
}
