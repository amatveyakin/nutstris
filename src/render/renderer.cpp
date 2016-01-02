#include "render/renderer.h"
#include "render/cubemesh.h"
#include "render/textureloader.h"
#include "render/matrixutil.h"
#include "render/shaderprogram.h"
#include "render/defs.h"

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
  math::Vec3f eyePos = { (FIELD_INDENT_RIGHT - FIELD_INDENT_LEFT) / 2.f, 0.0f,
                         (EYE_TO_FIELD + CUBE_SCALE / 2.f) - (FIELD_INDENT_TOP - (HUD_HEIGHT + FIELD_INDENT_BOTTOM)) / 2.f};
  math::Vec3f target = {(FIELD_INDENT_RIGHT - FIELD_INDENT_LEFT) / 2.f, 0.f , (FIELD_INDENT_TOP - (HUD_HEIGHT + FIELD_INDENT_BOTTOM)) / 2.f};
  auto matrixView = matrixutil::lookAt(eyePos, target, {0, 1, 0});
  auto matrixProj = matrixutil::perspective(ANGLE_FOV_Y, VP_ASPECT, 1.0f, 100.0f);
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