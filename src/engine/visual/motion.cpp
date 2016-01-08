#include "motion.h"

#include "math/basic.h"


namespace engine {

Motion::Motion(math::EasingFunction easingFunction__, FloatFieldCoords aimingShiftVector__, Time movingStartTime__, Time movingDuration__)
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
