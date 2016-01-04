#ifndef DATAFORMATS_H
#define DATAFORMATS_H
#include "math/vector.h"
#include "render/arraybuffer.h"

namespace render {
namespace dataformats {

struct PointWithNormal {
  math::Vec3f point;
  math::Vec3f normal;
  
  static void setUpLayout(ArrayBuffer<PointWithNormal>& buffer);
};
 
}
}
#endif // DATAFORMATS_H
