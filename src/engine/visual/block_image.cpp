#include "engine/player.h"


namespace engine {

BlockImage::BlockImage()
  : MovingObject(nullptr)
  , color_(COLORLESS)
  , mountEffect_(PIECE_MOUNTING_ANIMATION_TIME)
  , bonusEffect_(BONUS_FADING_DURATION)
{
}

BlockImage::BlockImage(VisualObject* parent__, Color color__, FieldCoords position__)
  : MovingObject(parent__)
  , color_(color__)
  , mountEffect_(PIECE_MOUNTING_ANIMATION_TIME)
  , bonusEffect_(BONUS_FADING_DURATION)
{
  placeAt(FloatFieldCoords(position__));
}


DisappearingLine::DisappearingLine(int row__, std::vector<BlockImage> blockImages__, Time disappearingStartTime__)
  : row_(row__)
  , blockImages_(std::move(blockImages__))
  , disappearingEffect_(LINE_DISAPPEAR_TIME)
{
  assert(blockImages_.size() == FIELD_WIDTH);
  disappearingEffect_.enable(disappearingStartTime__);
}

}  // namespace engine
