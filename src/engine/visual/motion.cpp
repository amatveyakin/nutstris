#include "motion.h"

#include "math/basic.h"


namespace engine {

float easeLinear(float v) {
  return v;
}
float easeInOutQuad(float v) {
  if (v < 0.5f)
    return math::sqr(v) * 2.f;
  else
    return 1.0f - math::sqr(1.0f - v) * 2.f;
}
float easeInOutQuart(float v) {
  if (v < 0.5f)
    return math::quart(v) * 8.f;
  else
    return 1.0f - math::quart(1.0f - v) * 8.f;
}
float easeOutBounce(float v) {
  const float a = 0.7f;
  const float b = 1.4f;
  if (v < a)
    return easeInOutQuad(v / a) * b;
  else
    return b - easeInOutQuad((v - a) / (1.0 - a)) * (b - 1.0f);
}


Motion::Motion(EasingFunction easingFunction__, FloatFieldCoords aimingShiftVector__, Time movingStartTime__, Time movingDuration__)
    : easingFunction_(easingFunction__)
    , aimingShiftVector_(aimingShiftVector__)
    , movingStartTime_(movingStartTime__)
    , movingDuration_(movingDuration__)
{
  assert(movingDuration__ > 0.0s);
}

bool Motion::started(Time currentTime) const {
  return currentTime >= movingStartTime_;
}
bool Motion::finished(Time currentTime) const {
  return currentTime >= movingStartTime_ + movingDuration_;
}

FloatFieldCoords Motion::shiftVector(Time currentTime) const {
  float progress = math::bound(0.0f, (currentTime - movingStartTime_) / movingDuration_, 1.0f);
  return aimingShiftVector_ * easingFunction_(progress);
}

}  // namespace engine
