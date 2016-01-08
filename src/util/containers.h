#pragma once

#include <bitset>
#include <cassert>


namespace util {

template <typename T, int size>
class FixedZeroBasedArray {
public:
  T& operator[](int index) {
    assert(0 <= index);
    assert(index < size);
    return elements_[index];
  }

  const T& operator[](int index) const {
    assert(0 <= index);
    assert(index < size);
    return elements_[index];
  }

protected:
  T elements_[size];
};


template <typename T, int firstRow, int firstCol, int lastRow, int lastCol>
class Fixed2DArray {
public:
  T& operator()(int row, int col) {
    assert(firstRow <= row);
    assert(row <= lastRow);
    assert(firstCol <= col);
    assert(col <= lastCol);
    return elements_[(row - firstRow) * nCols + (col - firstCol)];
  }

  const T& operator()(int row, int col) const {
    assert(firstRow <= row);
    assert(row <= lastRow);
    assert(firstCol <= col);
    assert(col <= lastCol);
    return elements_[(row - firstRow) * nCols + (col - firstCol)];
  }

protected:
  static const int nRows = lastRow - firstRow + 1;
  static const int nCols = lastCol - firstCol + 1;
  T elements_[nRows * nCols];
};


template <typename IndexT, IndexT elementBegin, IndexT elementEnd>
class StronglyTypedBitSet : private std::bitset<static_cast<size_t>(elementEnd) - static_cast<size_t>(elementBegin)> {
public:
  void add(IndexT index) {
    Parent::set(getUnderlyingIndex(index));
  }

  bool test(IndexT index) const {
    return Parent::test(getUnderlyingIndex(index));
  }

  bool any() const {
    return Parent::any();
  }

  bool none() const {
    return Parent::none();
  }

  void remove(IndexT index) {
    Parent::reset(getUnderlyingIndex(index));
  }

  void clear() {
    Parent::reset();
  }

private:
  using Parent = std::bitset<static_cast<size_t>(elementEnd) - static_cast<size_t>(elementBegin)>;

  size_t getUnderlyingIndex(IndexT index) const {
    assert(elementBegin <= index);
    assert(index < elementEnd);
    return static_cast<size_t>(index) - static_cast<size_t>(elementBegin);
  }
};

}  // namespace util
