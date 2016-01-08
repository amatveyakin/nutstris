#pragma once

#include "math/vector.h"


namespace math {

namespace L1 {
  template <int DIMENSION, typename ElementT>
  ElementT norm (Vector <DIMENSION, ElementT> a) {
    ElementT sum = 0;
    for (int i = 0; i < DIMENSION; ++i)
      sum += abs (a[i]);
    return sum;
  }

  template <int DIMENSION, typename ElementT>
  ElementT distance (Vector <DIMENSION, ElementT> a, Vector <DIMENSION, ElementT> b) {
    ElementT sum = 0;
    for (int i = 0; i < DIMENSION; ++i)
      sum += abs (a[i] - b[i]);
    return sum;
  }

  template <int DIMENSION, typename ElementT>
  Vector <DIMENSION, ElementT> normalized (Vector <DIMENSION, ElementT> a) {
    double normA = norm (a);
    assert (normA > MIN_NORM); // You a trying to normalize an almost zero vector
    return a / normA;
  }
}  // namespace L1

namespace L2 {
  template <int DIMENSION, typename ElementT>
  ElementT normSqr (Vector <DIMENSION, ElementT> a) {
    return dotProduct (a, a);
  }

  template <int DIMENSION, typename ElementT>
  ElementT norm (Vector <DIMENSION, ElementT> a) {
    return std::sqrt (normSqr (a));
  }

  template <int DIMENSION, typename ElementT>
  ElementT distanceSqr (Vector <DIMENSION, ElementT> a, Vector <DIMENSION, ElementT> b) {
  //   return euclideanNormSqr (a - b);
    ElementT sum = 0;
    for (int i = 0; i < DIMENSION; ++i)
      sum += sqr (a[i] - b[i]);
    return sum;
  }

  template <int DIMENSION, typename ElementT>
  ElementT distance (Vector <DIMENSION, ElementT> a, Vector <DIMENSION, ElementT> b) {
    return std::sqrt (distanceSqr (a, b));
  }

  template <int DIMENSION, typename ElementT>
  Vector <DIMENSION, ElementT> normalized (Vector <DIMENSION, ElementT> a) {
    double normA = norm (a);
    assert (normA > MIN_NORM); // You a trying to normalize an almost zero vector
    return a / normA;
  }
}  // namespace L2

namespace LInf {
  template <int DIMENSION, typename ElementT>
  ElementT norm (Vector <DIMENSION, ElementT> a) {
    ElementT max = 0;
    for (int i = 0; i < DIMENSION; ++i)
      max = max (max, abs (a[i]));
    return max;
  }

  template <int DIMENSION, typename ElementT>
  ElementT distance (Vector <DIMENSION, ElementT> a, Vector <DIMENSION, ElementT> b) {
    ElementT max = 0;
    for (int i = 0; i < DIMENSION; ++i)
      max = max (max, abs (a[i] - b[i]));
    return max;
  }

  template <int DIMENSION, typename ElementT>
  Vector <DIMENSION, ElementT> normalized (Vector <DIMENSION, ElementT> a) {
    double normA = norm (a);
    assert (normA > MIN_NORM); // You a trying to normalize an almost zero vector
    return a / normA;
  }
}  // namespace LInf

}  // namespace math
