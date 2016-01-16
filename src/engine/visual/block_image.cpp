#include "engine/player.h"


namespace engine {

BlockImage::BlockImage()
  : MovingObject(nullptr)
  , color_(COLORLESS)
  , bonusEffect_(BONUS_FADING_DURATION)
{
}

BlockImage::BlockImage(VisualObject* parent__, Color color__, FieldCoords position__)
  : MovingObject(parent__)
  , color_(color__)
  , bonusEffect_(BONUS_FADING_DURATION)
{
  placeAt(FloatFieldCoords(position__));
}


DisappearingLine::DisappearingLine(int row__, std::array<BlockImage, FIELD_WIDTH> blockImages__, Time disappearingStartTime__)
  : row_(row__)
  , blockImages_(std::move(blockImages__))
  , disappearingEffect_(LINE_DISAPPEAR_TIME)
{
  disappearingEffect_.enable(disappearingStartTime__);
}

}  // namespace engine
