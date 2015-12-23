#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include "render/renderer.h"

int main() {
  sf::Window window(sf::VideoMode(800, 600), "OpenGL");
  window.setVerticalSyncEnabled(true);

  // load resources, initialize the OpenGL states, ...

  Renderer renderer;
  // run the main loop
  bool running = true;
  while (running) {
    // handle events
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        // end the program
        running = false;
      }
      else if (event.type == sf::Event::Resized) {
        // adjust the viewport when the window is resized
        glViewport(0, 0, event.size.width, event.size.height);
      }
    }

    // clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw...
    renderer.render();

    // end the current frame (internally swaps the front and back buffers)
    window.display();
  }

  return 0;
}