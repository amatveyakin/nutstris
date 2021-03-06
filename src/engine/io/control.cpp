#include "control.h"

#include "engine/game.h"


namespace engine {

Time playerControlCooldown(PlayerControl control) {
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
