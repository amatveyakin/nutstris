#pragma once

#include "engine/visual/effect.h"
#include "engine/visual/object.h"


namespace engine {

class BlockImage : public MovingObject {
public:
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

  const NormalEffectType& bonusImage() const {
    return bonusImage_;
  }
  NormalEffectType& bonusImage() {
    return bonusImage_;
  }

private:
  Color color_;
  Bonus bonus_ = Bonus::None;
  NormalEffectType bonusImage_;
};


struct DisappearingLine {
  Color blockColor[FIELD_WIDTH];
  int row;
  Time startTime;
  Time duration;

  // TODO(Andrei): declare constants (?)
  double progress(Time currentTime) const {
    return math::bound(0.0, (currentTime - startTime) / duration, 1.0);
  }
};

}  // namespace engine
