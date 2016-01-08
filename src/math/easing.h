// Easing functions specify the rate of change of a parameter over time.
// Argument must be within [0, 1] interval. Result may be outside of this interval!
//
// See e.g. http://easings.net/

#pragma once

#include "math/basic.h"


namespace math {

using EasingFunction = double(*)(double);

double easeLinear(double v);

double easeInQuad(double v);
double easeOutQuad(double v);
double easeInOutQuad(double v);

double easeInCube(double v);
double easeOutCube(double v);
double easeInOutCube(double v);

double easeInQuart(double v);
double easeOutQuart(double v);
double easeInOutQuart(double v);

double easeInBounce(double v);
double easeOutBounce(double v);
double easeInOutBounce(double v);

}  // namespace math
