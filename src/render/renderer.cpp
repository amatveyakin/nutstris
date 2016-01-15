#include "render/renderer.h"
#include "render/cubemesh.h"
#include "render/texturedquad.h"
#include "render/texturefactory.h"
#include "render/matrixutil.h"
#include "render/shaderprogram.h"
#include "render/defs.h"

namespace render {

namespace {
const float kMaximalHintFaceOpacity = 0.3f;
const float kMaximalHintEdgeOpacity = 1.0f;

math::Vec4f getDiffuseColor(engine::Color c) {
  return{ c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, 1.0f };
}

const math::Vec4f kWhiteColor = { 1.0f, 1.0f, 1.0f, 1.0f };

math::Vec4f getSpecularColor(engine::Color c) {
  auto specularColor = (getDiffuseColor(c) + kWhiteColor) / 2.0f;
  specularColor.w() = 128.0f;
  return specularColor;
}

float fieldToWorldX(double fieldX) {
  return (float(fieldX) - (engine::FIELD_WIDTH - 1.0f) / 2.0f) * render::CUBE_SCALE;
}

float fieldToWorldY(double fieldY) {
  return (float(fieldY) - (engine::FIELD_HEIGHT - 1.0f) / 2.0f) * render::CUBE_SCALE;
}

math::Mat4x4f fieldPosToWorldPos(double fieldX, double fieldY) {
  return math::Mat4x4f::translationMatrix({ fieldToWorldX(fieldX), fieldToWorldY(fieldY), 0.0f }) *
    render::matrixutil::scale(render::CUBE_SCALE);
}

std::vector<dataformats::CubeInstance> blockImagesToCubeInstances(const std::vector<engine::BlockImage>& blockImages,
                                                                  engine::Time now) {
  std::vector<dataformats::CubeInstance> result;
  for (auto& block : blockImages) {
    float bonusProgress = float(block.bonusImage().progress(now));
    auto bonusIndex = (bonusProgress > 0.5f) ? int(block.bonus()) : 0;
    auto cubeScale = math::abs(2.f * bonusProgress - 1.f);
    auto scaleMatrix = render::matrixutil::scale(cubeScale);
    auto pos2d = block.absolutePosition(now);
    result.push_back({ fieldPosToWorldPos(pos2d.x(), pos2d.y()) * scaleMatrix,
                       getDiffuseColor(block.color()), getSpecularColor(block.color()), bonusIndex });
  }
  return result;
}
}  // namespace

Renderer::Renderer() {
  glewInit();
  glEnable ( GL_DEPTH_TEST );
  glDisable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  cubeMesh_ = std::make_unique<CubeMesh>();

  wall_ = std::make_unique<TexturedQuad>( CUBE_SCALE * engine::FIELD_WIDTH  * (1.0f + CUBE_SCALE * (engine::FIELD_HEIGHT / 2.0f + 0.5f) / EYE_TO_FIELD),
                                          CUBE_SCALE * engine::FIELD_HEIGHT * (1.0f + CUBE_SCALE * (engine::FIELD_HEIGHT / 2.0f + 0.5f) / EYE_TO_FIELD),
                                          1.0f,  1.0f);
  bonusesTexture_ = TextureFactory::createBonusesTexture();
  wallTexture_ = TextureFactory::createWallTexture();

  lightsSettingsBuffer_ = std::make_unique<UniformBuffer<dataformats::LightsSettings>>();
}

Renderer::~Renderer() {
}

void Renderer::renderGame ( const engine::GameRound& game, engine::Time now ) {
  for ( size_t iPlayer = 0; iPlayer < game.players().size(); ++iPlayer ) {
    auto& player = *game.players()[iPlayer];
    if ( player.active() ) {
      prepareToDrawPlayer_ ( iPlayer, player, now );
      renderPlayer_ ( player, now );
    }
  }
}

void Renderer::prepareToDrawPlayer_(size_t iPlayer, const engine::Player& player, engine::Time now) {
  glViewport(playerViewports_[iPlayer].x, playerViewports_[iPlayer].y,
             playerViewports_[iPlayer].width, playerViewports_[iPlayer].height);

  dataformats::LightsSettings l;
  l.ambientColor = math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
  l.diffuseColor = math::Vec4f(0.8f, 0.8f, 0.8f, 0.8f);
  l.specularColor = math::Vec4f(0.6f, 0.6f, 0.6f, 100.0f);
  l.direction = math::Vec4f(0.0f, 0.0f, -1.0f, 0.0f);
  l.lightType = 1;
  l.brightness = 1.0f;
  lightsSettingsBuffer_->setData({ l });

  cubeMesh_->getShaderProgram().setUniformBuffer("LightsSettings", *lightsSettingsBuffer_);
  cubeMesh_->getShaderProgram().setUniform("gVP", getViewProjection_());
  cubeMesh_->getShaderProgram().setUniform("gGlobalRotation", getGlobalRotation_(player, now));
  cubeMesh_->getShaderProgram().setUniform("gBonusesTextureArray", bonusesTexture_->getTextureSlotIndex());
  cubeMesh_->getShaderProgram().setUniform("gHintAreaClipPlane", math::Vec4f(0.0, 1.0, 0.0, -MAX_WORLD_FIELD_HEIGHT / 2.0));
  cubeMesh_->getShaderProgram().setUniform("gWaveProgress", getWaveProgress(player, now));
  cubeMesh_->getShaderProgram().setUniform("gTruncatedCubesProgress", float(player.visualEffects.semicubes.progress(now)));

  wall_->getShaderProgram().setUniformBuffer("LightsSettings", *lightsSettingsBuffer_);
  wall_->getShaderProgram().setUniform("gVP", getViewProjection_());
  wall_->getShaderProgram().setUniform("gWorld", math::Mat4x4f::translationMatrix({ 0.0f, 0.0f, -CUBE_SCALE * engine::FIELD_HEIGHT / 2.0f }) *
                                       matrixutil::rotation({ 0.0, 1.0, 0.0 }, math::kPi));
  wall_->getShaderProgram().setUniform("gDiffuseMap", wallTexture_->getTextureSlotIndex());
  wall_->getShaderProgram().setUniform("gDiffuseMapLayer", player.backgroundSeed % int(wallTexture_->getTextureCount()));
}

void Renderer::renderPlayer_ ( const engine::Player& player, engine::Time now ) {
  renderWall_();
  renderDisappearingLines_(player.disappearingLines, now);
  renderCubes_(blockImagesToCubeInstances(player.lyingBlockImages, now), kMaximalHintFaceOpacity, kMaximalHintEdgeOpacity, false);
  renderCubes_(blockImagesToCubeInstances(player.fallingBlockImages, now), kMaximalHintFaceOpacity, kMaximalHintEdgeOpacity, true);
  renderHint_(player, now);
}

void Renderer::renderDisappearingLines_(const std::vector<engine::DisappearingLine>& lines, engine::Time now ) {
  for ( size_t iDisappearingLine = 0; iDisappearingLine < lines.size(); ++iDisappearingLine ) {
    auto& currentLine = lines[iDisappearingLine];
    math::Vec4f clippingPlane = { 2.0f * ( iDisappearingLine % 2 ) - 1.0f, 1.0f, 1.0f,
                                  1.5f * ( 2.f * float( currentLine.progress ( now ) ) - 1.f )
                                };
    std::vector<dataformats::CubeInstance> lineCubesData;
    for ( size_t x = 0; x < engine::FIELD_WIDTH; ++x )
      lineCubesData.push_back ( {fieldPosToWorldPos ( x, currentLine.row ), 
                               getDiffuseColor(currentLine.blockColor[x]), getSpecularColor(currentLine.blockColor[x]), 0} );
    renderCubes_ ( lineCubesData, kMaximalHintFaceOpacity, kMaximalHintEdgeOpacity, false, clippingPlane );
  }
}

void Renderer::renderHint_(const engine::Player& player, engine::Time now) {
  std::vector<dataformats::CubeInstance> hintCubesData;
  for (size_t i = 0; i < player.nextPieces()[0].nBlocks(); ++i) {
    hintCubesData.push_back({ fieldPosToWorldPos(player.nextPieces()[0].absoluteCoords(i).x(), player.nextPieces()[0].absoluteCoords(i).y()),
                              getDiffuseColor(player.nextPieces()[0].color()), getSpecularColor(player.nextPieces()[0].color()), 0 });
  }
  
  auto faceOpacity = kMaximalHintFaceOpacity * float(player.visualEffects.hintMaterialization.progress(now));
  auto edgeOpacity = float(player.visualEffects.hintAppearance.progress(now));
  renderCubes_(hintCubesData, faceOpacity, edgeOpacity, true);
}

void Renderer::renderCubes_(const std::vector<dataformats::CubeInstance>& cubesData, float faceOpacity, float edgeOpacity, 
                            bool falling, math::Vec4f clipPlane) {
  if (falling)
    cubeMesh_->getShaderProgram().setUniform("gCubeSmoothness", 0.55f);
  else
    cubeMesh_->getShaderProgram().setUniform("gCubeSmoothness", 0.35f);

  cubeMesh_->getShaderProgram().setUniform ( "gClipPlane", clipPlane );
  cubeMesh_->getShaderProgram().setUniform("gFaceOpacity", faceOpacity);
  cubeMesh_->getShaderProgram().setUniform("gEdgeOpacity", edgeOpacity);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  cubeMesh_->render ( cubesData );
  glCullFace(GL_BACK);
  cubeMesh_->render(cubesData);
  glDisable(GL_CULL_FACE);
}

math::Mat4x4f Renderer::getViewProjection_() const {
  math::Vec3f eyePos = { ( FIELD_INDENT_RIGHT - FIELD_INDENT_LEFT ) / 2.f, 0.0f,
                         ( EYE_TO_FIELD + CUBE_SCALE / 2.f ) - ( FIELD_INDENT_TOP - ( HUD_HEIGHT + FIELD_INDENT_BOTTOM ) ) / 2.f
                       };
  math::Vec3f target = { ( FIELD_INDENT_RIGHT - FIELD_INDENT_LEFT ) / 2.f, 0.f , ( FIELD_INDENT_TOP - ( HUD_HEIGHT + FIELD_INDENT_BOTTOM ) ) / 2.f};
  auto matrixView = matrixutil::lookAt ( eyePos, target, {0, 1, 0} );
  auto matrixProj = matrixutil::perspective ( ANGLE_FOV_Y, VP_ASPECT, 1.0f, 100.0f );
  auto VP = matrixProj * matrixView;
  return VP;
}

math::Mat4x4f Renderer::getGlobalRotation_(const engine::Player & player, engine::Time now) const {
  float rotatingProgress = player.visualEffects.rotatingField.progress(now) * math::kPi;
  float rotatingAngle = (-sin(4.0f * rotatingProgress) * 0.15f + rotatingProgress) * 2.0f;
  float flippedScreenAngle = player.visualEffects.flippedScreen.progress(now) * math::kPi;
  return matrixutil::rotation({ 0, 1, 0 }, rotatingAngle) * matrixutil::rotation({ 1, 0, 0 }, flippedScreenAngle);
}

float Renderer::getWaveProgress(const engine::Player & player, engine::Time now) const {
  return player.visualEffects.wave.progress(now) * 2.0f * math::kPi;
}

void Renderer::renderWall_ () {
  wall_->render();
}

void Renderer::updatePlayerViewports(int nPlayers, int screenWidth, int screenHeight) {
  playerViewports_ = createPlayerViewports ( nPlayers, screenWidth, screenHeight );
}

}  // namespace render
