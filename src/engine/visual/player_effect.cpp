#include "player_effect.h"

#include "engine/player.h"


namespace engine {

PlayerVisualEffects::PlayerVisualEffects()
  : hintAppearance(HINT_APPERAING_TIME)
  , hintMaterialization(HINT_MATERIALIZATION_TIME)
  , fieldCleaning(BONUS_CLEAR_FIELD_DURATION)
  , playerDying(PLAYER_DYING_ANIMATION_TIME)
  , noHint(BONUS_REMOVING_HINT_DURATION)
  , flippedScreen(BONUS_FLIPPING_SCREEN_DURATION)
  , rotatingField(BONUS_ROTATING_SCREEN_PERIOD)
  , semicubes(BONUS_CUTTING_BLOCKS_DURATION)
  , wave(BONUS_WAVE_PERIOD)
  , lantern(BONUS_LANTERN_FADING_TIME)
{
}

PieceTheftEffect::PieceTheftEffect()
  : NormalEffectType(BONUS_PIECE_THEFT_DURATION) {}

}  // namespace engine
