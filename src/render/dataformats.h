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
  math::Vec3f   color;
  int           textureIndex;

  static void setUpLayout(ArrayBuffer<CubeInstance>& buffer);
};

struct LightsSettings {
  math::Vec3f   direction;
};

 
}
}
