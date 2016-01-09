#pragma once

#include "engine/defs.h"
#include "util/containers.h"


namespace engine {

const int    SKY_HEIGHT = MAX_PIECE_SIZE;
// MAX_PIECE_SIZE / 2  is enough for  WALL_WIDTH  in most cases, but that's perfectly safe
const int    WALL_WIDTH = MAX_PIECE_SIZE - 1;  // TODO: may be, abadon walls and modify Field::operator(int, int)  ?

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
  Color color_;
  Bonus bonus_;
};


struct Field : public util::Fixed2DArray<FieldCell, -WALL_WIDTH, -WALL_WIDTH,
                                         FIELD_HEIGHT + SKY_HEIGHT, FIELD_WIDTH + WALL_WIDTH>
{
  Field();

  // TODO: swap and rename arguments
  FieldCell& operator()(int row, int col)
  {
    return Fixed2DArray<FieldCell, -WALL_WIDTH, -WALL_WIDTH,
                        FIELD_HEIGHT + SKY_HEIGHT, FIELD_WIDTH + WALL_WIDTH>::
                        operator()(row, col);
  }

  const FieldCell& operator()(int row, int col) const
  {
    return Fixed2DArray<FieldCell, -WALL_WIDTH, -WALL_WIDTH,
                        FIELD_HEIGHT + SKY_HEIGHT, FIELD_WIDTH + WALL_WIDTH>::
                        operator()(row, col);
  }

  FieldCell& operator()(FieldCoords position)
  {
    return operator()(position.y(), position.x());
  }

  const FieldCell& operator()(FieldCoords position) const
  {
    return operator()(position.y(), position.x());
  }

  void clear();
};

}  // namespace engine
