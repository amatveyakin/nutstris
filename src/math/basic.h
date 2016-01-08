#pragma once

#include <cstdlib>
#include <limits>


namespace math {

static double constexpr kPi = 3.141592653589793238463;

template <typename NumericT>
NumericT min (NumericT x, NumericT y) {
  return (x < y) ? x : y;
}
template <typename NumericT>
NumericT min (NumericT x, NumericT y, NumericT z)
{
  return min (x, min (y, z));
}
template <typename NumericT>
NumericT min (NumericT x, NumericT y, NumericT z, NumericT t)
{
  return min (x, min (y, z, t));
}

template <typename NumericT>
NumericT max (NumericT x, NumericT y) {
  return (x < y) ? y : x;
}
template <typename NumericT>
NumericT max (NumericT x, NumericT y, NumericT z)
{
  return max (x, max (y, z));
}
template <typename NumericT>
NumericT max (NumericT x, NumericT y, NumericT z, NumericT t)
{
  return max (x, max (y, z, t));
}

template <typename NumericT>
NumericT bound (NumericT min_val, NumericT value, NumericT max_val) {
  return min (max_val, max (min_val, value));
}

template <typename NumericT>
NumericT abs (NumericT x) {
  return (x >= 0) ? x : (-x);
}

template <typename NumericT>
NumericT sqr (NumericT x) {
  return x * x;
}
template <typename NumericT>
NumericT cube (NumericT x) {
  return x * x * x;
}
template <typename NumericT>
NumericT quart (NumericT x) {
  return x * x * x * x;
}

template <typename NumericT>
NumericT sgn (NumericT x) {
  return (x > 0) - (x < 0);
}


static_assert ((-3) / 2 == -1, "If this assertion fails, divFloored and modFloored functions will not work");

// WARNING: divisor must be positive
template <typename NumericT>
inline NumericT divFloored (NumericT dividend, NumericT divisor) {
  static_assert (std::numeric_limits <NumericT>::is_integer, "Integral type expected.");
  return (dividend - (dividend < 0) * (divisor - 1)) / divisor;
}

// WARNING: divisor must be positive
template <typename NumericT>
inline NumericT modFloored (NumericT dividend, NumericT divisor) {
  static_assert (std::numeric_limits <NumericT>::is_integer, "Integral type expected.");
  NumericT stupidMod = dividend % divisor;
  return stupidMod + (stupidMod < 0) * divisor;
}

template <typename NumericT>
NumericT gcd (NumericT a, NumericT b) {
  static_assert (std::numeric_limits <NumericT>::is_integer, "Integral type expected.");
  while (b != 0) {
    NumericT c = a % b;
    a = b;
    b = c;
  }
  return a;
}

template <typename NumericT>
NumericT lcm (NumericT a, NumericT b) {
  static_assert (std::numeric_limits <NumericT>::is_integer, "Integral type expected.");
  // The ``result = a * b / gcd (a, b)'' code may cause an integer overflow
  NumericT result = a / gcd (a, b);
  result *= b;
  return result;
}


inline double randomRange(double minValue, double maxValue) {
  // TODO(Andrei): use std::uniform_real_distribution
  return minValue + double(rand()) * (maxValue - minValue) / RAND_MAX;
}

}  // namespace math
