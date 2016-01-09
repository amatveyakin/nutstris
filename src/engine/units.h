#pragma once

#include <chrono>


namespace engine {

using namespace std::chrono_literals;

using Time = std::chrono::duration<double>;  // seconds


class Speed {
public:
  constexpr Speed() {}  // constructs zero speed

  constexpr static Speed zero() { return Speed(); }

  constexpr double operator*(Time t) const            { return t * numerator_ / kDenominator_; }
  constexpr friend double operator*(Time t, Speed s)  { return s.operator*(t); }

  constexpr Speed operator*(double c) const           { return Speed(numerator_ / c); }
  constexpr Speed operator/(double c) const           { return Speed(numerator_ / c); }
  constexpr friend Speed operator*(double c, Speed s) { return s * c; }

  constexpr friend Speed operator/(double d, Time t);

  constexpr bool operator==(Speed rhs) const  { return numerator_ == rhs.numerator_; }
  constexpr bool operator!=(Speed rhs) const  { return numerator_ != rhs.numerator_; }
  constexpr bool operator< (Speed rhs) const  { return numerator_ <  rhs.numerator_; }
  constexpr bool operator> (Speed rhs) const  { return numerator_ >  rhs.numerator_; }
  constexpr bool operator<=(Speed rhs) const  { return numerator_ <= rhs.numerator_; }
  constexpr bool operator>=(Speed rhs) const  { return numerator_ >= rhs.numerator_; }

private:
  constexpr explicit Speed(double numerator__) : numerator_(numerator__) {}

  double numerator_ = 0.0;
  constexpr static Time kDenominator_ = 1.0s;
};

constexpr inline Speed operator/(double d, Time t) {
  return Speed(d / (t / Speed::kDenominator_));
}

}  // namespace engine
