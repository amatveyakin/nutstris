// TODO(Andrei): Clean up SFML functions from other files

// TODO(Andrei): User-defind literals for time in seconds

#ifndef ENGINE_DEFS_H
#define ENGINE_DEFS_H

#include <limits>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "math/vector.h"


namespace engine {

// TODO(Andrei): Fix naming
const int    FIELD_WIDTH = 10;
const int    FIELD_HEIGHT = 20;
const int    MAX_BLOCKS = FIELD_WIDTH * FIELD_HEIGHT;
const int    MAX_BLOCK_IMAGES = MAX_BLOCKS;

const int    MAX_PIECE_SIZE = 4;
const int    MAX_BLOCKS_IN_PIECE = MAX_PIECE_SIZE * MAX_PIECE_SIZE;

const int    MAX_PLAYERS = 4;


using FieldCoords = math::Vec2i;
using FloatFieldCoords = math::Vec2f;
using CompareFieldCoords = math::LexicographicCompareVec2i;


using Time = sf::Time;
const Time NEVER_HAPPENED = sf::microseconds(std::numeric_limits<sf::Int64>::min());
const Time WILL_NEVER_HAPPEN = sf::microseconds(std::numeric_limits<sf::Int64>::max());

// TODO(Andrei): Adequate Speed type
using Speed = float;


using Color = sf::Color;
const Color COLORLESS = Color::Transparent;

inline Color colorFromFloat(float r, float g, float b, float a) {
  auto component = [](float v){ return math::bound(0, static_cast<int>(v * 255), 255); };
  return Color(component(r), component(g), component(b), component(a));
}


enum class Bonus {
  // no bonus
  None,

  // buffs
  EnlargeHintQueue,
  PieceTheft,

  // kind sorceries
  Heal,
  SlowDown,
  ClearField,

  // debuffs
  FlippedScreen,
  RotatingScreen,
  Wave,
  Lantern,
  CrazyPieces,
  TruncatedBlocks,
  NoHint,

  // evil sorceries
  SpeedUp,
//  FlipField,
};

const Bonus  FIRST_BONUS = Bonus::None;
const Bonus  FIRST_REAL_BONUS = Bonus::EnlargeHintQueue;
const Bonus  LAST_BONUS = Bonus::SpeedUp;
const Bonus  LAST_REAL_BONUS = Bonus::SpeedUp;
const int    N_BONUSES = static_cast<int>(LAST_BONUS) - static_cast<int>(FIRST_BONUS) + 1;
const int    N_REAL_BONUSES = static_cast<int>(LAST_REAL_BONUS) - static_cast<int>(FIRST_REAL_BONUS) + 1;


const int N_PLAYER_KEYS = 7;

enum class PlayerKey
{
  Left,
  Right,
  RotateCCW,
  RotateCW,
  Down,
  Drop,
  NextVictim,
};

}  // namespace engine

#endif  // ENGINE_DEFS_H
