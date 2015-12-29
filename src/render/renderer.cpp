#include "render/renderer.h"
#include "render/cubemesh.h"
#include "render/textureloader.h"
#include "render/matrixutil.h"
#include "render/shaderprogram.h"

namespace render {

Renderer::Renderer() {
  glewInit();
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  cubeMesh_ = std::make_unique<CubeMesh>();
  textureLoader_ = std::make_unique<TextureLoader>("resources/textures/bonuses/");
}

Renderer::~Renderer() {

}

void Renderer::render(const std::vector<CubeMesh::PerCubeData>& cubesData) {
  auto matrixView = matrixutil::lookAt({0.0, 1.5, 10.5}, {0, 0, 0}, {0, 1, 0});
  auto matrixProj = matrixutil::perspective(M_PI / 2.5f, 4.0 / 3.0, 0.01f, 100.0f);
  auto VP = matrixProj * matrixView;
  cubeMesh_->getShaderProgram().setUniform("VP", VP);
  cubeMesh_->getShaderProgram().setUniform("textures", 0);
  cubeMesh_->render(cubesData);
}

}
