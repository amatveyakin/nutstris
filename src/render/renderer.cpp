#include "render/renderer.h"
#include "render/cubemesh.h"

namespace render {

Renderer::Renderer() {
  glewInit();
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  cubeMesh_ = std::make_unique<CubeMesh>();
}

Renderer::~Renderer() {

}

void Renderer::render() {
  cubeMesh_->render();
}

}