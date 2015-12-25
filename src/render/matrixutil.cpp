#include "matrixutil.h"

#include "math/metric.h"

namespace render {
namespace matrixutil {

math::Vec4f extend(math::Vec3f v, float elt) {
  return {v.x(), v.y(), v.z(), elt};
}

math::Mat4x4f createFromColumns(math::Vec3f e1, math::Vec3f e2, math::Vec3f e3, math::Vec3f translation) {
    return math::Mat4x4f({ extend(e1), extend(e2), extend(e3), extend(translation, 1.0f)});
}

math::Mat4x4f rotation(math::Vec3f axis, float angle) {
  axis = math::L2::normalized(axis);
  math::Vec3f basis[3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  math::Vec3f transformed[3] = {};
  for (int i = 0; i < 3; ++i)
  {
    auto along = math::dotProduct(basis[i], axis);
    auto orthogonal = basis[i] - along * axis;
    auto orthogonalRotatedBy90Degrees = math::crossProduct(axis, orthogonal);
    transformed[i] = along * axis + orthogonal * cos(angle) + orthogonalRotatedBy90Degrees * sin(angle);
  }
  return createFromColumns(transformed[0], transformed[1], transformed[2], {0, 0, 0});
}


math::Mat4x4f lookAt(math::Vec3f eye, math::Vec3f center, math::Vec3f up) {
  auto eyeToCenter = math::L2::normalized(center - eye);
  auto horizontal = math::L2::normalized(math::crossProduct(eyeToCenter, up));
  auto vertical = math::L2::normalized(math::crossProduct(horizontal, eyeToCenter));
  auto inversed = createFromColumns(horizontal, vertical, -eyeToCenter, {0, 0, 0});
  auto result = inversed.transposed();
  result = result * math::Mat4x4f::translationMatrix(-eye);
  return result;
}

math::Mat4x4f perspective(float fov, float aspect, float clipNear, float clipFar) {
  auto result = math::Mat4x4f::zeroMatrix();
  auto f = 1.0f / tan(fov / 2.0f);
  result(0, 0) = f;
  result(1, 1) = f / aspect;
  result(2, 2) = (clipFar + clipNear) / (clipNear - clipFar);
  result(3, 2) = -1.0f;
  result(2, 3) = 2.0f * clipFar * clipNear / (clipNear - clipFar);
  return result;
}


}
}
