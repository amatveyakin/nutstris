#pragma once

#include <memory>

#include "engine/defs.h"


namespace engine {

// Function specifying the rate of change of a parameter over time.
// Argument must be within [0.0, 1.0] interval. Result may be outside of this interval!
using EasingFunction = float(*)(float);

float easeLinear(float v);
float easeInOutQuad(float v);
float easeInOutQuart(float v);
float easeOutBounce(float v);

class Motion
{
public:
  Motion(EasingFunction easingFunction__, FloatFieldCoords aimingShiftVector__, Time movingStartTime__, Time movingDuration__);

  bool started(Time currentTime) const;
  bool finished(Time currentTime) const;

  FloatFieldCoords shiftVector(Time currentTime) const;

private:
  EasingFunction easingFunction_ = nullptr;
  FloatFieldCoords aimingShiftVector_;
  Time movingStartTime_ = 0.0s;
  Time movingDuration_ = 0.0s;
};

}  // namespace engine
