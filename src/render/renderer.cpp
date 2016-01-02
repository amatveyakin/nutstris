#include "render/renderer.h"
#include "render/cubemesh.h"
#include "render/textureloader.h"
#include "render/matrixutil.h"
#include "render/shaderprogram.h"

namespace render {

Renderer::Renderer() {
  glewInit();
  glEnable(GL_DEPTH_TEST);
  cubeMesh_ = std::make_unique<CubeMesh>();
  textureLoader_ = std::make_unique<TextureLoader>("resources/textures/bonuses/");
}

Renderer::~Renderer() {

}

void Renderer::prepareToDrawPlayer ( size_t iPlayer ) {
  glViewport(playerViewports_[iPlayer].x, playerViewports_[iPlayer].y,
             playerViewports_[iPlayer].width, playerViewports_[iPlayer].height);
}


void Renderer::render(const std::vector<CubeMesh::PerCubeData>& cubesData, math::Vec4f clipPlane) {
  auto matrixView = matrixutil::lookAt({0.0, 1.5, 10.5}, {0, 0, 0}, {0, 1, 0});
  auto matrixProj = matrixutil::perspective(M_PI / 2.5f, 4.0 / 3.0, 0.01f, 100.0f);
  auto VP = matrixProj * matrixView;
  cubeMesh_->getShaderProgram().setUniform("gVP", VP);
  cubeMesh_->getShaderProgram().setUniform("gGlobalRotation", math::Mat4x4f::identityMatrix());
  cubeMesh_->getShaderProgram().setUniform("gBonusesTextureArray", 0);
  cubeMesh_->getShaderProgram().setUniform("gClipPlane", clipPlane);
  cubeMesh_->render(cubesData);
}

void Renderer::updatePlayerViewports ( int nPlayers, int screenWidth, int screenHeight ) {
  playerViewports_ = createPlayerViewports(nPlayers, screenWidth, screenHeight);
}


}
