#pragma once

#include "math/vector.h"
#include "math/matrix.h"
#include "render/arraybuffer.h"

namespace render {
namespace dataformats {

struct UncoloredVertex {
  math::Vec3f point;
  math::Vec3f normal;

  static void setUpLayout(ArrayBuffer<UncoloredVertex>& buffer);
};

struct TexturedVertex
{
  math::Vec3f point;
  math::Vec3f normal;
  math::Vec2f texCoord;

  static void setUpLayout(ArrayBuffer<TexturedVertex>& buffer);
};

struct CubeInstance
{
  math::Mat4x4f modelToWorld;
  math::Vec4f   diffuseColor;
  math::Vec4f   specularColor;
  int           textureIndex;
  float         cubeSmoothness;

  static void setUpLayout(ArrayBuffer<CubeInstance>& buffer);
};

struct LightsSettings {
  math::Vec4f position;
  math::Vec4f direction;
  math::Vec4f ambientColor;
  math::Vec4f diffuseColor;
  math::Vec4f specularColor;
  math::Vec3f att;
  float spotPower;
  float range;
  int   lightType;  //0 - off, 1 - parallel, 2 - point, 3 - spot 
  float brightness;
};


}
}
