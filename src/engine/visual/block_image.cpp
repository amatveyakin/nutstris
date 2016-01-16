#include "engine/player.h"


namespace engine {

BlockImage::BlockImage(VisualObject* parent__, Color color__, FieldCoords position__)
    : MovingObject(parent__)
    , color_(color__)
    , bonusImage_(BONUS_FADING_DURATION)
{
  placeAt(FloatFieldCoords(position__));
}

}  // namespace engine
