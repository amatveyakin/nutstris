#include "dataformats.h"

namespace render {
namespace dataformats {

void PointWithNormal::setUpLayout ( ArrayBuffer<PointWithNormal>& buffer ) {
  buffer.setUpFloatAttribute(0, 3, false, offsetof(PointWithNormal, point));
  buffer.setUpFloatAttribute(1, 3, false, offsetof(PointWithNormal, normal));
}

}
}