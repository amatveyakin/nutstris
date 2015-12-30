#ifndef MATH_VECTOR_H
#define MATH_VECTOR_H

#include <cassert>
#include <cstddef>
#include <cmath>
#include <limits>
#include <algorithm>
#include <ostream>

#include "math/basic.h"


namespace math {

struct Uninitialized {};

// TODO: turn it into template
static const double MIN_NORM = 1e-8;

// TODO: delete
#define DECLARE_DERIVED(Type__) \
  private: \
    const Type__& derived () const    { return static_cast <const Type__&> (*this); } \
    Type__& derived ()                { return static_cast <Type__&> (*this); }


template <int DIMENSION, typename ElementT>
class VectorBase {
public:
  VectorBase() : m_elements() {}
  explicit VectorBase(Uninitialized) {}
  explicit VectorBase(const ElementT* elements)   { std::copy(elements, elements + DIMENSION, m_elements); }
  template <typename OtherElementT>
  explicit VectorBase(const VectorBase<DIMENSION, OtherElementT>& source) {
    for (int i = 0; i < DIMENSION; ++i)
      m_elements[i] = static_cast<ElementT>(source[i]);
  }

  ElementT at(size_t index) const           { return m_elements [index]; }
  ElementT& at(size_t index)                { return m_elements [index]; }
  ElementT operator[](size_t index) const   { return m_elements [index]; }
  ElementT& operator[](size_t index)        { return m_elements [index]; }

  const ElementT* data() const              { return m_elements; }
  ElementT* data()                          { return m_elements; }

  const ElementT* cbegin() const            { return m_elements; }
  ElementT* begin()                         { return m_elements; }
  const ElementT* cend() const              { return m_elements + DIMENSION; }
  ElementT* end()                           { return m_elements + DIMENSION; }

  bool operator== (const VectorBase& a) const {
    static_assert (std::numeric_limits <ElementT>::is_integer, "Comparing floating point numbers with operator== is discouraged!");
    for (int i = 0; i < DIMENSION; ++i) {
      if (m_elements[i] != a.m_elements[i])
        return false;
    }
    return true;
  }
  bool operator!= (const VectorBase& a) const {
    static_assert (std::numeric_limits <ElementT>::is_integer, "Comparing floating point numbers with operator!= is discouraged!");
    return !(&this == a);
  }

protected:
  ElementT m_elements [DIMENSION];

  template <int, typename, typename>
  friend class CommonVectorLinearOperations;
};


template <int DIMENSION, typename VectorT, typename ElementT>
class CommonVectorLinearOperations {
public:
  // in-place operators
  VectorT& operator+= (VectorT a) {
    for (int i = 0; i < DIMENSION; ++i)
      derived ().m_elements[i] += a.m_elements[i];
    return derived ();
  }
  VectorT& operator-= (VectorT a) {
    for (int i = 0; i < DIMENSION; ++i)
      derived ().m_elements[i] -= a.m_elements[i];
    return derived ();
  }
  VectorT& operator*= (ElementT q) {
    for (int i = 0; i < DIMENSION; ++i)
      derived ().m_elements[i] *= q;
    return derived ();
  }
  VectorT& operator/= (ElementT q) {
    for (int i = 0; i < DIMENSION; ++i)
      derived ().m_elements[i] /= q;
    return derived ();
  }
  VectorT& operator%= (ElementT q) {
    for (int i = 0; i < DIMENSION; ++i)
      derived ().m_elements[i] %= q;
    return derived ();
  }

  // unary operators
  VectorT operator- () const {
    return VectorT () - derived ();
  }

  // binary operators
  VectorT operator+ (VectorT a) const {
    VectorT result (derived ());
    result += a;
    return result;
  }
  VectorT operator- (VectorT a) const {
    VectorT result (derived ());
    result -= a;
    return result;
  }
  VectorT operator* (ElementT q) const {
    VectorT result (derived ());
    result *= q;
    return result;
  }
  VectorT operator/ (ElementT q) const {
    VectorT result (derived ());
    result /= q;
    return result;
  }
  VectorT operator% (ElementT q) const {
    VectorT result (derived ());
    result %= q;
    return result;
  }
  friend VectorT operator* (ElementT q, VectorT a) {
    return a * q;
  }

  DECLARE_DERIVED (VectorT)
};

template <int DIMENSION, typename VectorT, typename ElementT>
class VectorLinearOperations : public CommonVectorLinearOperations <DIMENSION, VectorT, ElementT> { };

template <int DIMENSION, typename VectorT>
class VectorLinearOperations <DIMENSION, VectorT, int> : public CommonVectorLinearOperations <DIMENSION, VectorT, int> {
public:
  // in-place operators
  VectorT& applyDivFloored (int q) {
    for (int i = 0; i < DIMENSION; ++i)
      derived ()[i] = divFloored (derived ()[i], q);
    return derived ();
  }
  VectorT& applyModFloored (int q) {
    for (int i = 0; i < DIMENSION; ++i)
      derived ()[i] = modFloored (derived ()[i], q);
    return derived ();
  }

  // binary operators
  VectorT divFloored (int q) const {
    VectorT result (derived ());
    result.applyDivFloored (q);
    return result;
  }
  VectorT modFloored (int q) const {
    VectorT result (derived ());
    result.applyModFloored (q);
    return result;
  }
  // TODO: add a combined divModFloored function

  DECLARE_DERIVED (VectorT)
};


template <int VECTOR_DIMENSION, typename ElementT>
class Vector {
  Vector() = delete;
};

template <typename ElementT>
class Vector <2, ElementT> : public VectorBase              <2, ElementT>,
                             public VectorLinearOperations  <2, Vector <2, ElementT>, ElementT>
{
private:
  typedef VectorBase <2, ElementT> Parent;

public:
  using Parent::Parent;
  Vector () {}
  Vector (ElementT x__, ElementT y__)                 { setCoordinates (x__, y__); }

  ElementT x () const                       { return Parent::at (0); }
  ElementT& x ()                            { return Parent::at (0); }
  ElementT y () const                       { return Parent::at (1); }
  ElementT& y ()                            { return Parent::at (1); }

  void setCoordinates (ElementT x__, ElementT y__)  { x () = x__;  y () = y__; }

  static Vector e1 ()                       { return Vector (1, 0); }
  static Vector e2 ()                       { return Vector (0, 1); }
  static Vector e_i (int i)                 { Vector result;  result[i] = 1;  return result; }
};

template <typename ElementT>
class Vector <3, ElementT> : public VectorBase              <3, ElementT>,
                             public VectorLinearOperations  <3, Vector <3, ElementT>, ElementT>
{
private:
  typedef VectorBase <3, ElementT> Parent;

public:
  using Parent::Parent;
  Vector () {}
  Vector (ElementT x__, ElementT y__, ElementT z__)   { setCoordinates (x__, y__, z__); }

  ElementT x () const                       { return Parent::at (0); }
  ElementT& x ()                            { return Parent::at (0); }
  ElementT y () const                       { return Parent::at (1); }
  ElementT& y ()                            { return Parent::at (1); }
  ElementT z () const                       { return Parent::at (2); }
  ElementT& z ()                            { return Parent::at (2); }

  void setCoordinates (ElementT x__, ElementT y__, ElementT z__)  { x () = x__;  y () = y__;  z () = z__; }

  // TODO: generate other subsets and permutations
  Vector <2, ElementT> xy () const          { return Vector <2, ElementT> (x (), y ()); }

  static Vector e1 ()                       { return Vector (1, 0, 0); }
  static Vector e2 ()                       { return Vector (0, 1, 0); }
  static Vector e3 ()                       { return Vector (0, 0, 1); }
  static Vector e_i (int i)                 { Vector result;  result[i] = 1;  return result; }
};

template <typename ElementT>
class Vector <4, ElementT> : public VectorBase              <4, ElementT>,
                             public VectorLinearOperations  <4, Vector <4, ElementT>, ElementT>
{
private:
  typedef VectorBase <4, ElementT> Parent;

public:
  using Parent::Parent;
  Vector () {}
  Vector (ElementT x__, ElementT y__, ElementT z__, ElementT w__)   { setCoordinates (x__, y__, z__, w__); }

  ElementT x () const                       { return Parent::at (0); }
  ElementT& x ()                            { return Parent::at (0); }
  ElementT y () const                       { return Parent::at (1); }
  ElementT& y ()                            { return Parent::at (1); }
  ElementT z () const                       { return Parent::at (2); }
  ElementT& z ()                            { return Parent::at (2); }
  ElementT w () const                       { return Parent::at (3); }
  ElementT& w ()                            { return Parent::at (3); }

  void setCoordinates (ElementT x__, ElementT y__, ElementT z__, ElementT w__)  { x () = x__;  y () = y__;  z () = z__;  w () = w__; }

  static Vector e1 ()                       { return Vector (1, 0, 0, 0); }
  static Vector e2 ()                       { return Vector (0, 1, 0, 0); }
  static Vector e3 ()                       { return Vector (0, 0, 1, 0); }
  static Vector e4 ()                       { return Vector (0, 0, 0, 1); }
  static Vector e_i (int i)                 { Vector result;  result[i] = 1;  return result; }
};


template <int DIMENSION, typename ElementT>
ElementT dotProduct (Vector <DIMENSION, ElementT> a, Vector <DIMENSION, ElementT> b) {
  ElementT sum = 0;
  for (int i = 0; i < DIMENSION; ++i)
    sum += a[i] * b[i];
  return sum;
}

// template <int DIMENSION, typename ElementT>
// Vector <2, ElementT> crossProduct (Vector <2, ElementT> a) {
//   return Vector <2, ElementT> (-a.y, a.x);
// }

template <typename ElementT>
Vector <3, ElementT> crossProduct (Vector <3, ElementT> a, Vector <3, ElementT> b) {
  return Vector <3, ElementT> (a.y() * b.z() - a.z() * b.y(),  a.z() * b.x() - a.x() * b.z(),  a.x() * b.y() - a.y() * b.x());
}


template <int DIMENSION, typename ElementT>
std::ostream& operator<< (std::ostream& os, Vector <DIMENSION, ElementT> a) {
  os << "(";
  for (int i = 0; i < DIMENSION - 1; ++i)
    os << a[i] << ", ";
  os << a[DIMENSION - 1] << ")";
  return os;
}


template <int DIMENSION, typename ElementT>
struct LexicographicCompareVectors {
  static_assert (std::numeric_limits <ElementT>::is_integer,
                 "Using floating-point values as keys for a set or a map is almost certainly a bad idea.");
  bool operator() (Vector <DIMENSION, ElementT> a, Vector <DIMENSION, ElementT> b) const {
    for (int i = 0; i < DIMENSION; ++i) {
      if (a[i] < b[i])
        return true;
      else if (a[i] > b[i])
        return false;
    }
    return false;
  }
};


typedef Vector <2, unsigned char> Vec2uc;
typedef Vector <3, unsigned char> Vec3uc;
typedef Vector <4, unsigned char> Vec4uc;

typedef Vector <2, int>     Vec2i;
typedef Vector <3, int>     Vec3i;
typedef Vector <4, int>     Vec4i;

typedef Vector <2, float>   Vec2f;
typedef Vector <3, float>   Vec3f;
typedef Vector <4, float>   Vec4f;

typedef Vector <2, double>  Vec2d;
typedef Vector <3, double>  Vec3d;
typedef Vector <4, double>  Vec4d;


struct LexicographicCompareVec2i : LexicographicCompareVectors <2, int> { };
struct LexicographicCompareVec3i : LexicographicCompareVectors <3, int> { };
struct LexicographicCompareVec4i : LexicographicCompareVectors <4, int> { };

}  // namespace math

#endif  // MATH_VECTOR_H
