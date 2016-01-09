#pragma once

#include <bitset>
#include <cassert>


namespace util {

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
