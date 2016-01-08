#pragma once

#include <math/matrix.h>

namespace render {
namespace matrixutil {
  math::Vec4f extend(math::Vec3f v, float elt = 0.0f);
  math::Mat4x4f createFromColumns(math::Vec3f e1, math::Vec3f e2, math::Vec3f e3, math::Vec3f translation);

  math::Mat4x4f rotation(math::Vec3f axis, float angle);
  math::Mat4x4f scale(float coeff);

  math::Mat4x4f lookAt(math::Vec3f eye, math::Vec3f center, math::Vec3f up);
  math::Mat4x4f perspective(float fov, float aspect, float clipNear, float clipFar);
}
}
