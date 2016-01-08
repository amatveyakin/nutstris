#include "easing.h"

#include "util/macro.h"


#define DEFINE_EASE_OUT_AND_IN_OUT_FROM_IN(suffix)         \
double easeOut ## suffix(double v) {                        \
  return easeOutFromIn(easeIn ## suffix, v);                \
}                                                           \
double easeInOut ## suffix(double v) {                      \
  return easeInOutFromIn(easeIn ## suffix, v);              \
}

namespace math {

namespace {
double easeOutFromIn(EasingFunction easeIn, double v) {
  return 1. - easeIn(1. - v);
}
double easeInOutFromIn(EasingFunction easeIn, double v) {
  if (v < 0.5)
    return easeIn(v * 2.) / 2.;
  else
    return 1. - easeIn((1. - v) * 2.) / 2.;
}
}  // namespace


double easeLinear(double v) {
  return v;
}

double easeInQuad(double v) {
  return math::sqr(v);
}
DEFINE_EASE_OUT_AND_IN_OUT_FROM_IN(Quad)

double easeInCube(double v) {
  return math::cube(v);
}
DEFINE_EASE_OUT_AND_IN_OUT_FROM_IN(Cube)

double easeInQuart(double v) {
  return math::quart(v);
}
DEFINE_EASE_OUT_AND_IN_OUT_FROM_IN(Quart)

// TODO(Andrei) this a mixture of Bounce and Elastic - fix that
double easeInBounce(double v) {
  const double hitPoint = 0.7;
  const double amplitude = 1.4;
  if (v < hitPoint)
    return easeInQuad(v / hitPoint) * amplitude;
  else
    return amplitude - easeOutQuad((v - hitPoint) / (1.0 - hitPoint)) * (amplitude - 1.0);
}
DEFINE_EASE_OUT_AND_IN_OUT_FROM_IN(Bounce)


}  // namespace math
