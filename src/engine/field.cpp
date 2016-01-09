#include "field.h"


Field::Field()
{
  // ``Floor''
  for (int row = BORDERED_FIELD_ROW_BEGIN; row < 0; ++row)
    for (int col = BORDERED_FIELD_COL_BEGIN; col < BORDERED_FIELD_COL_END; ++col)
      operator()(row, col).setBlock(COLORLESS);
  // ``Walls''
  for (int row = BORDERED_FIELD_ROW_BEGIN; row < BORDERED_FIELD_ROW_END; ++row)
  {
    for (int col = BORDERED_FIELD_COL_BEGIN; col < 0; ++col)
      operator()(row, col).setBlock(COLORLESS);
    for (int col = FIELD_WIDTH; col < BORDERED_FIELD_COL_END; ++col)
      operator()(row, col).setBlock(COLORLESS);
  }
  // ``Sky''
  for (int row = FIELD_HEIGHT; row < BORDERED_FIELD_ROW_END; ++row)
    for (int col = 0; col < FIELD_WIDTH; ++col)
      operator()(row, col).clear();
}

void Field::clear()
{
  for (int row = 0; row < FIELD_HEIGHT; ++row)
    for (int col = 0; col < FIELD_WIDTH; ++col)
      operator()(row, col).clear();
}
