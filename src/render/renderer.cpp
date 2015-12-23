#include "render/renderer.h"
#include "render/cubemesh.h"

namespace render {

Renderer::Renderer() {
  glewInit();
  cubeMesh_ = std::make_unique<CubeMesh>();
}

Renderer::~Renderer() {

}

void Renderer::render() {
  cubeMesh_->render();
}

}