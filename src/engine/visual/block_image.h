#pragma once

#include "engine/visual/effect.h"
#include "engine/visual/object.h"


namespace engine {

class BlockImage : public MovingObject {
public:
  BlockImage();
  BlockImage(VisualObject* parent__, Color color__, FieldCoords position__);

  Color color() const {
    return color_;
  }

  Bonus bonus() const {
    return bonus_;
  }
  void setBonus(Bonus bonus__) {
    bonus_ = bonus__;
  }

  const NormalEffectType& bonusEffect() const {
    return bonusEffect_;
  }
  NormalEffectType& bonusEffect() {
    return bonusEffect_;
  }

private:
  Color color_;
  Bonus bonus_ = Bonus::None;
  NormalEffectType bonusEffect_;
};


class DisappearingLine {
public:
  DisappearingLine(int row__, std::array<BlockImage, FIELD_WIDTH> blockImages__, Time disappearingStartTime__);

  int row() const {
    return row_;
  }
  int& row() {
    return row_;
  }

  const std::array<BlockImage, FIELD_WIDTH>& blockImages() const {
    return blockImages_;
  }

  const NormalEffectType& disappearingEffect() const {
    return disappearingEffect_;
  }

private:
  int row_;
  std::array<BlockImage, FIELD_WIDTH> blockImages_;
  NormalEffectType disappearingEffect_;
};

}  // namespace engine
