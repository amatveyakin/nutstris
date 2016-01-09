#pragma once

#include <array>

#include "engine/defs.h"
#include "util/containers.h"


namespace engine {

class FieldCell {
public:
  bool blocked() const {
    return blocked_;
  }
  Color color() const {
    return color_;
  }
  Bonus bonus() const {
    return bonus_;
  }

  void clear() {
    blocked_ = false;
  }
  void setBlock(Color color__, Bonus bonus__ = Bonus::None) {
    blocked_ = true;
    color_ = color__;
    bonus_ = bonus__;
  }
  void setBonus(Bonus bonus__) {
    bonus_ = bonus__;
  }

private:
  bool blocked_ = false;
  Color color_ = COLORLESS;
  Bonus bonus_ = Bonus::None;
};


struct Field {
public:
  Field() {
    blockedCell_.setBlock(COLORLESS);
  }

  const FieldCell& operator()(FieldCoords position) const {
    if (insideField_(position))
      return cells_[index_(position)];
    else if (overField_(position))
      return freeCell_;
    else
      return blockedCell_;
  }
  FieldCell& mutableCell(FieldCoords position) {
    assert(insideField_(position));
    return cells_[index_(position)];
  }

private:
  std::array<FieldCell, FIELD_WIDTH * FIELD_HEIGHT> cells_;
  FieldCell blockedCell_;
  FieldCell freeCell_;

  bool insideField_(FieldCoords position) const {
    return 0 <= position.x() && position.x() < FIELD_WIDTH &&
           0 <= position.y() && position.y() < FIELD_HEIGHT;
  }
  bool overField_(FieldCoords position) const {
    return 0 <= position.x() && position.x() < FIELD_WIDTH &&
           FIELD_HEIGHT <= position.y();
  }
  size_t index_(FieldCoords position) const { return position.x() * FIELD_HEIGHT + position.y(); }
};

}  // namespace engine
