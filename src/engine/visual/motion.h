#pragma once

#include "engine/defs.h"
#include "math/basic.h"


namespace engine {

class LinearMotion
{
public:
  LinearMotion(FloatFieldCoords aimingShiftVector__, Time movingStartTime__, Time movingDuration__)
      : aimingShiftVector_(aimingShiftVector__)
      , movingStartTime_(movingStartTime__)
      , movingDuration_(movingDuration__)
  {
    assert(movingDuration__ > 0.0s);
  }

  bool begun(Time currentTime) const {
    return currentTime >= movingStartTime_;
  }

  bool finished(Time currentTime) const {
    return currentTime >= movingStartTime_ + movingDuration_;
  }

  FloatFieldCoords shiftVector(Time currentTime) const {
    float multiplier = math::bound(0.0f, (currentTime - movingStartTime_) / movingDuration_, 1.0f);
    return aimingShiftVector_ * multiplier;
  }

private:
  FloatFieldCoords aimingShiftVector_;
  Time movingStartTime_;
  Time movingDuration_;
};

}  // namespace engine
