#pragma once

#include <array>

#include "engine/units.h"
#include "engine/io/keyboard.h"


namespace engine {

static constexpr int kNumPlayerControls = 7;

enum PlayerControl {
  Left,
  Right,
  RotateCCW,
  RotateCW,
  Down,
  Drop,
  NextVictim,
};

using PlayerControls = std::array<Keyboard::Key, kNumPlayerControls>;

Time playerControlCooldown(PlayerControl control);


static constexpr int kNumGlobalControls = 1;

enum GlobalControl {
  SayHi,  // just for testing
};

using GlobalControls = std::array<Keyboard::Key, kNumGlobalControls>;

Time globalControlCooldown(GlobalControl control);

}  // namespace engine
