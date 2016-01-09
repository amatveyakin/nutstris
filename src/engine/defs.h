// TODO(Andrei): Clean up SFML functions from other files

#pragma once

#include <limits>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "engine/units.h"
#include "math/vector.h"


namespace engine {

using namespace std::chrono_literals;

// TODO(Andrei): Fix naming
const int    FIELD_WIDTH = 10;
const int    FIELD_HEIGHT = 20;

const int    MAX_PIECE_SIZE = 4;

const int    MAX_PLAYERS = 4;


using FieldCoords = math::Vec2i;
using FloatFieldCoords = math::Vec2d;
using CompareFieldCoords = math::LexicographicCompareVec2i;


using Color = sf::Color;
const Color COLORLESS = Color::Transparent;

inline Color colorFromFloat(double r, double g, double b, double a) {
  auto component = [](double v){ return math::bound(0, static_cast<int>(v * 255), 255); };
  return Color(component(r), component(g), component(b), component(a));
}

}  // namespace engine
