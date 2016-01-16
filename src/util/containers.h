#pragma once

#include <bitset>
#include <cassert>
#include <map>


namespace util {

template<typename MapT>
typename MapT::mapped_type mapExtract(MapT& map, const typename MapT::key_type& key) {
  auto it = map.find(key);
  assert(it != map.end());
  typename MapT::mapped_type value = std::move(it->second);
  map.erase(it);
  return value;
}

template<typename MapT>
void mapInsertUnique(MapT& map, typename MapT::value_type keyValue) {
  auto insertionResult = map.insert(std::move(keyValue));
  assert(insertionResult.second);
}

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
