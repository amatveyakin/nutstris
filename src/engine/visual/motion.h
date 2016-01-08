#pragma once

#include <memory>

#include "engine/defs.h"
#include "math/easing.h"


namespace engine {

class Motion
{
public:
  Motion(math::EasingFunction easingFunction__, FloatFieldCoords aimingShiftVector__, Time movingStartTime__, Time movingDuration__);

  bool started(Time currentTime) const;
  bool finished(Time currentTime) const;

  FloatFieldCoords shiftVector(Time currentTime) const;

private:
  math::EasingFunction easingFunction_ = nullptr;
  FloatFieldCoords aimingShiftVector_;
  Time movingStartTime_ = 0.0s;
  Time movingDuration_ = 0.0s;
};

}  // namespace engine
