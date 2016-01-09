#include "control.h"

#include "engine/engine.h"


namespace engine {

Time playerControlCooldown(PlayerControl control) {
  const Time kMoveControlCooldown = 0.12s;
  const Time kRotateControlCooldown = 0.15s;
  switch (control) {
  case Left      :
  case Right     : return 0.12s;
  case RotateCCW :
  case RotateCW  : return 0.15s;
  case Down      : return PIECE_FORCED_LOWERING_ANIMATION_TIME;
  case Drop      : return 0.25s;
  case NextVictim: return 0.2s;
  }
  abort();
}


Time globalControlCooldown(GlobalControl /*control*/) {
  return 0.25s;
}

}  // namespace engine
