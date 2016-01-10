#include "render/renderer.h"
#include "render/cubemesh.h"
#include "render/texturedquad.h"
#include "render/texturefactory.h"
#include "render/matrixutil.h"
#include "render/shaderprogram.h"
#include "render/defs.h"

namespace render {

namespace {
math::Vec3f ColorToVec3 ( engine::Color c ) {
  return {c.r / 255.0f, c.g / 255.0f, c.b / 255.0f};
}

float fieldToWorldX (double fieldX ) {
  return ( float(fieldX) - (engine::FIELD_WIDTH - 1.0f) / 2.0f) * render::CUBE_SCALE;
}

float fieldToWorldY (double fieldY ) {
  return ( float(fieldY) - ( engine::FIELD_HEIGHT - 1.0f ) / 2.0f ) * render::CUBE_SCALE;
}

math::Mat4x4f fieldPosToWorldPos ( double fieldX, double fieldY ) {
  return math::Mat4x4f::translationMatrix ( {fieldToWorldX ( fieldX ), fieldToWorldY ( fieldY ), 0.0f} ) *
         render::matrixutil::scale ( render::CUBE_SCALE );
}
}


Renderer::Renderer() {
  glewInit();
  glEnable ( GL_DEPTH_TEST );
  cubeMesh_ = std::make_unique<CubeMesh>();
  wall_ = std::make_unique<TexturedQuad>( CUBE_SCALE * engine::FIELD_WIDTH  * (1.0f + CUBE_SCALE * (engine::FIELD_HEIGHT / 2.0f + 0.5f) / EYE_TO_FIELD),
                                          CUBE_SCALE * engine::FIELD_HEIGHT * (1.0f + CUBE_SCALE * (engine::FIELD_HEIGHT / 2.0f + 0.5f) / EYE_TO_FIELD),
                                          1.0f,  1.0f);
  bonusesTexture_ = TextureFactory::createBonusesTexture();
  wallTexture_ = TextureFactory::createWallTexture();
}

Renderer::~Renderer() {
}

void Renderer::renderGame ( engine::Game& game, engine::Time now ) {
  for ( size_t iPlayer = 0; iPlayer < game.participants.size(); ++iPlayer ) {
    auto player = game.participants[iPlayer];
    if ( player->active ) {
      prepareToDrawPlayer_ ( iPlayer );
      renderPlayer_ ( *player, now );
    }
  }
}

void Renderer::prepareToDrawPlayer_ ( size_t iPlayer ) {
  glViewport ( playerViewports_[iPlayer].x, playerViewports_[iPlayer].y,
               playerViewports_[iPlayer].width, playerViewports_[iPlayer].height );
}

void Renderer::renderPlayer_ ( engine::Player& player, engine::Time now ) {
  std::vector<dataformats::CubeInstance> cubesData;
  auto addBlocks = [&cubesData, now] ( std::vector<engine::BlockImage>& blockImages ) {
    for ( auto& block : blockImages ) {
      float bonusProgress = float(block.bonusImage().progress ( now ));
      auto bonusIndex = ( bonusProgress > 0.5f ) ? int ( block.bonus() ) : 0;
      auto cubeScale = math::abs( 2.f * bonusProgress - 1.f );
      auto scaleMatrix = render::matrixutil::scale ( cubeScale );
      auto pos2d = block.absolutePosition ( now );
      cubesData.push_back ( {fieldPosToWorldPos ( pos2d.x(), pos2d.y() ) * scaleMatrix, ColorToVec3 ( block.color() ), bonusIndex} );
    }
  };
  addBlocks ( player.lyingBlockImages );
  addBlocks ( player.fallingBlockImages );
  renderCubes_ ( cubesData, getGlobalRotation_(player, now) );
  renderDisappearingLines_( player.disappearingLines, getGlobalRotation_(player, now), now );
  renderWall_( player );
}

void Renderer::renderDisappearingLines_(const std::vector<engine::DisappearingLine>& lines, 
                                        math::Mat4x4f globalRotation, engine::Time now ) {
  for ( size_t iDisappearingLine = 0; iDisappearingLine < lines.size(); ++iDisappearingLine ) {
    auto& currentLine = lines[iDisappearingLine];
    math::Vec4f clippingPlane = { 2.0f * ( iDisappearingLine % 2 ) - 1.0f, 1.0f, 1.0f,
                                  1.5f * ( 2.f * float( currentLine.progress ( now ) ) - 1.f )
                                };
    std::vector<dataformats::CubeInstance> lineCubesData;
    for ( size_t x = 0; x < engine::FIELD_WIDTH; ++x )
      lineCubesData.push_back ( {fieldPosToWorldPos ( x, currentLine.row ), ColorToVec3 ( currentLine.blockColor[x] ), 0} );
    renderCubes_ ( lineCubesData, globalRotation, clippingPlane );
  }
}

void Renderer::renderCubes_ ( const std::vector<dataformats::CubeInstance>& cubesData, math::Mat4x4f globalRotation, math::Vec4f clipPlane ) {
  cubeMesh_->getShaderProgram().setUniform ( "gVP", getViewProjection_() );
  cubeMesh_->getShaderProgram().setUniform("gGlobalRotation", globalRotation);
  cubeMesh_->getShaderProgram().setUniform ( "gBonusesTextureArray", bonusesTexture_->getTextureSlotIndex() );
  cubeMesh_->getShaderProgram().setUniform ( "gClipPlane", clipPlane );
  cubeMesh_->render ( cubesData );
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

math::Mat4x4f Renderer::getGlobalRotation_(engine::Player & player, engine::Time now) const {
  float rotatingProgress = player.visualEffects.rotatingField.progress(now) * math::kPi;
  float rotatingAngle = (-sin(4.0f * rotatingProgress) * 0.15f + rotatingProgress) * 2.0f;
  float flippedScreenAngle = player.visualEffects.flippedScreen.progress(now) * math::kPi;
  return matrixutil::rotation({ 0, 1, 0 }, rotatingAngle) * matrixutil::rotation({ 1, 0, 0 }, flippedScreenAngle);
}

void Renderer::renderWall_ ( engine::Player& player) {
  wall_->getShaderProgram().setUniform( "gVP", getViewProjection_() );
  wall_->getShaderProgram().setUniform( "gWorld", math::Mat4x4f::translationMatrix({0.0f, 0.0f, -CUBE_SCALE * engine::FIELD_HEIGHT / 2.0f}));
  wall_->getShaderProgram().setUniform ( "gDiffuseMap", wallTexture_->getTextureSlotIndex() );
  wall_->getShaderProgram().setUniform ( "gDiffuseMapLayer", player.backgroundSeed % wallTexture_->getTextureCount() );
  wall_->render();
}

void Renderer::updatePlayerViewports ( int nPlayers, int screenWidth, int screenHeight ) {
  playerViewports_ = createPlayerViewports ( nPlayers, screenWidth, screenHeight );
}

}
