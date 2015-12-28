#ifndef MATH_MATRIX_H
#define MATH_MATRIX_H
#include <array>
#include "math/vector.h"


namespace math {

// A column-major order matrix
template <int N_ROWS, int N_COLS, typename ElementT>
class MatrixBase {
public:
  MatrixBase() : m_elements() {}
  explicit MatrixBase(Uninitialized) {}
  explicit MatrixBase(const ElementT* elements)   { std::copy(elements, elements + N_ELEMENTS, m_elements); }
  explicit MatrixBase(const std::array<Vector<N_COLS, ElementT>, N_ROWS>& columns) {
    for (int i = 0; i < N_ROWS; ++i) {
      for (int j = 0; j < N_COLS; ++j)
        this->at(i, j) = columns[j][i];
    }
  }

  const ElementT* elements () const             { return m_elements; }
  ElementT* elements ()                         { return m_elements; }

  ElementT operator() (int row, int col) const  { return m_elements [internalIndex_(row, col)]; }
  ElementT& operator() (int row, int col)       { return m_elements [internalIndex_(row, col)]; }
  ElementT at (int row, int col) const          { return m_elements [internalIndex_(row, col)]; }
  ElementT& at (int row, int col)               { return m_elements [internalIndex_(row, col)]; }

protected:
  static const int N_ELEMENTS = N_ROWS * N_COLS;
  ElementT m_elements [N_ELEMENTS];

  size_t internalIndex_(int row, int col) const { return col * N_ROWS + row; }

  template <int, typename, typename>
  friend class CommonVectorLinearOperations;
};


// Rectangle matrix
template <int N_ROWS, int N_COLS, typename ElementT>
class Matrix : public MatrixBase <N_ROWS, N_COLS, ElementT>,
               public VectorLinearOperations <N_ROWS * N_COLS, Matrix <N_ROWS, N_COLS, ElementT>, ElementT>
{
private:
  typedef MatrixBase <N_ROWS, N_COLS, ElementT> Parent;

public:
  using Parent::Parent;
};

// Square matrix
template <int SIZE, typename ElementT>
class Matrix <SIZE, SIZE, ElementT> : public MatrixBase <SIZE, SIZE, ElementT>,
                                      public VectorLinearOperations <SIZE * SIZE, Matrix <SIZE, SIZE, ElementT>, ElementT>
{
private:
  typedef MatrixBase <SIZE, SIZE, ElementT> Parent;

public:
  using Parent::Parent;

  static Matrix identityMatrix () {
    Matrix result;
    for (int i = 0; i < SIZE; ++i)
      result (i, i) = 1;
    return result;
  }

  static Matrix translationMatrix (Vector <SIZE - 1, ElementT> shift) {
    Matrix result = Matrix::identityMatrix ();
    for (int i = 0; i < SIZE - 1; ++i)
      result (i, SIZE - 1) = shift[i];
    return result;
  }

  Matrix transposed() const {
    Matrix result{Uninitialized{}};
    for (int i = 0; i < SIZE; ++i) {
      for (int j = 0; j < SIZE; ++j)
        result(i, j) = this->at(j, i);
    }
    return result;
  }
};


template <int DIMENSION_IN, int DIMENSION_OUT, typename ElementT>
Vector <DIMENSION_OUT, ElementT> operator* (Matrix <DIMENSION_OUT, DIMENSION_IN, ElementT> matrix,
                                            Vector <DIMENSION_IN, ElementT> vector)
{
  Vector <DIMENSION_OUT, ElementT> result;
  for (int row = 0; row < DIMENSION_OUT; ++row) {
    ElementT sum = 0;
    for (int col = 0; col < DIMENSION_IN; ++col)
      sum += matrix (row, col) * vector [col];
    result [row] = sum;
  }
  return result;
}

template <int LHS_ROWS, int COMMON_SIZE, int RHS_COLS, typename ElementT>
Matrix <LHS_ROWS, RHS_COLS, ElementT> operator* (Matrix <LHS_ROWS, COMMON_SIZE, ElementT> lhs,
                                                 Matrix <COMMON_SIZE, RHS_COLS, ElementT> rhs)
{
  Matrix <LHS_ROWS, RHS_COLS, ElementT> result;
  for (int row = 0; row < LHS_ROWS; ++row) {
    for (int col = 0; col < RHS_COLS; ++col) {
      ElementT sum = 0;
      for (int i = 0; i < COMMON_SIZE; ++i)
        sum += lhs (row, i) * rhs (i, col);
      result (row, col) = sum;
    }
  }
  return result;
}


typedef Matrix <2, 2, float>  Mat2x2f;
typedef Matrix <3, 3, float>  Mat3x3f;
typedef Matrix <4, 4, float>  Mat4x4f;

typedef Matrix <2, 2, double> Mat2x2d;
typedef Matrix <3, 3, double> Mat3x3d;
typedef Matrix <4, 4, double> Mat4x4d;

}  // namespace math

#endif // MATH_MATRIX_H
