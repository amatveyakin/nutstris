#pragma once

#include "engine/visual/effect.h"
#include "engine/visual/object.h"


namespace engine {

class BlockImage : public MovingObject {
public:
  BlockImage();
  BlockImage(VisualObject* parent__, Color color__, FieldCoords position__);

  BlockImage(const BlockImage&) = delete;
  BlockImage& operator=(const BlockImage&) = delete;
  BlockImage(BlockImage&&) = default;
  BlockImage& operator=(BlockImage&&) = default;

  Color color() const {
    return color_;
  }

  Bonus bonus() const {
    return bonus_;
  }
  void setBonus(Bonus bonus__) {
    bonus_ = bonus__;
  }

  const NormalEffectType& mountEffect() const {
    return mountEffect_;
  }
  NormalEffectType& mountEffect() {
    return mountEffect_;
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
  NormalEffectType mountEffect_;
  NormalEffectType bonusEffect_;
};


class DisappearingLine {
public:
  DisappearingLine(int row__, std::vector<BlockImage> blockImages__, Time disappearingStartTime__);

  int row() const {
    return row_;
  }
  int& row() {
    return row_;
  }

  const std::vector<BlockImage>& blockImages() const {
    return blockImages_;
  }

  const NormalEffectType& disappearingEffect() const {
    return disappearingEffect_;
  }

private:
  int row_;
  std::vector<BlockImage> blockImages_;
  NormalEffectType disappearingEffect_;
};

}  // namespace engine
