#include "dataformats.h"

namespace render {
namespace dataformats {

void UncoloredVertex::setUpLayout ( ArrayBuffer<UncoloredVertex>& buffer ) {
  buffer.setUpFloatAttribute(0, 3, false, offsetof(UncoloredVertex, point));
  buffer.setUpFloatAttribute(1, 3, false, offsetof(UncoloredVertex, normal));
}

void TexturedVertex::setUpLayout ( ArrayBuffer< TexturedVertex >& buffer ) {
  buffer.setUpFloatAttribute(0, 3, false, offsetof(TexturedVertex, point));
  buffer.setUpFloatAttribute(1, 3, false, offsetof(TexturedVertex, normal));
  buffer.setUpFloatAttribute(2, 2, false, offsetof(TexturedVertex, texCoord));
}

void CubeInstance::setUpLayout(ArrayBuffer<CubeInstance>& buffer) {
  const int textureIDLayout = 2;
  const int transformLayout = 4;
  const int colorLayout = 8;

  for (int i = 0; i < 4; i++)
    buffer.setUpFloatAttribute(transformLayout + i, 4, false, sizeof(float) * 4 * i);

  buffer.setUpFloatAttribute(colorLayout, 3, false, offsetof(CubeInstance, color));
  buffer.setUpIntAttribute(textureIDLayout, 1, offsetof(CubeInstance, textureIndex));
}


}
}