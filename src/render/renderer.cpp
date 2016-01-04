#include "render/renderer.h"
#include "render/cubemesh.h"
#include "render/textureloader.h"
#include "render/matrixutil.h"
#include "render/shaderprogram.h"
#include "render/defs.h"

namespace render {

namespace {
math::Vec3f ColorToVec3 ( engine::Color c ) {
  return {c.r / 255.0f, c.g / 255.0f, c.b / 255.0f};
}

float fieldToWorldX ( float fieldX ) {
  return ( fieldX - ( engine::FIELD_WIDTH - 1.0f ) / 2.0f ) * render::CUBE_SCALE;
}

float fieldToWorldY ( float fieldY ) {
  return ( fieldY - ( engine::FIELD_HEIGHT - 1.0f ) / 2.0f ) * render::CUBE_SCALE;
}

math::Mat4x4f fieldPosToWorldPos ( int fieldX, int fieldY ) {
  return math::Mat4x4f::translationMatrix ( {fieldToWorldX ( fieldX ), fieldToWorldY ( fieldY ), 0.0f} ) *
         render::matrixutil::scale ( render::CUBE_SCALE );
}
}


Renderer::Renderer() {
  glewInit();
  glEnable ( GL_DEPTH_TEST );
  cubeMesh_ = std::make_unique<CubeMesh>();
  textureLoader_ = std::make_unique<TextureLoader> ( "resources/textures/bonuses/" );
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
      auto bonusProgress = block.bonusImage.progress ( now );
      auto bonusIndex = ( bonusProgress > 0.5f ) ? int ( block.bonus ) : 0;
      auto cubeScale = fabs ( 2 * bonusProgress - 1 );
      auto scaleMatrix = render::matrixutil::scale ( cubeScale );
      auto pos2d = block.position ( now );
      cubesData.push_back ( {fieldPosToWorldPos ( pos2d.x(), pos2d.y() ) * scaleMatrix, ColorToVec3 ( block.color ), bonusIndex} );
    }
  };
  addBlocks ( player.lyingBlockImages );
  addBlocks ( player.fallingBlockImages );
  renderCubes_ ( cubesData );
  renderDisappearingLines_( player.disappearingLines, now );
}

void Renderer::renderDisappearingLines_ ( const std::vector<engine::DisappearingLine>& lines, engine::Time now ) {
  for ( size_t iDisappearingLine = 0; iDisappearingLine < lines.size(); ++iDisappearingLine ) {
    auto& currentLine = lines[iDisappearingLine];
    math::Vec4f clippingPlane = { 2.0f * ( iDisappearingLine % 2 ) - 1.0f, 1.0f, 1.0f,
                                  1.5f * ( 2.f * currentLine.progress ( now ) - 1.f )
                                };
    std::vector<dataformats::CubeInstance> lineCubesData;
    for ( size_t x = 0; x < engine::FIELD_WIDTH; ++x )
      lineCubesData.push_back ( {fieldPosToWorldPos ( x, currentLine.row ), ColorToVec3 ( currentLine.blockColor[x] ), 0} );
    renderCubes_ ( lineCubesData, clippingPlane );
  }
}


void Renderer::renderCubes_ ( const std::vector<dataformats::CubeInstance>& cubesData, math::Vec4f clipPlane ) {
  math::Vec3f eyePos = { ( FIELD_INDENT_RIGHT - FIELD_INDENT_LEFT ) / 2.f, 0.0f,
                         ( EYE_TO_FIELD + CUBE_SCALE / 2.f ) - ( FIELD_INDENT_TOP - ( HUD_HEIGHT + FIELD_INDENT_BOTTOM ) ) / 2.f
                       };
  math::Vec3f target = { ( FIELD_INDENT_RIGHT - FIELD_INDENT_LEFT ) / 2.f, 0.f , ( FIELD_INDENT_TOP - ( HUD_HEIGHT + FIELD_INDENT_BOTTOM ) ) / 2.f};
  auto matrixView = matrixutil::lookAt ( eyePos, target, {0, 1, 0} );
  auto matrixProj = matrixutil::perspective ( ANGLE_FOV_Y, VP_ASPECT, 1.0f, 100.0f );
  auto VP = matrixProj * matrixView;
  cubeMesh_->getShaderProgram().setUniform ( "gVP", VP );
  cubeMesh_->getShaderProgram().setUniform ( "gGlobalRotation", math::Mat4x4f::identityMatrix() );
  cubeMesh_->getShaderProgram().setUniform ( "gBonusesTextureArray", 0 );
  cubeMesh_->getShaderProgram().setUniform ( "gClipPlane", clipPlane );
  cubeMesh_->render ( cubesData );
}

void Renderer::updatePlayerViewports ( int nPlayers, int screenWidth, int screenHeight ) {
  playerViewports_ = createPlayerViewports ( nPlayers, screenWidth, screenHeight );
}

}