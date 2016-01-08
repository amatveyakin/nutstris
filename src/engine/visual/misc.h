#pragma once

#include "engine/visual/effect.h"
#include "engine/visual/object.h"


namespace engine {

class BlockImage : public MovingObject {
public:
  BlockImage(VisualObject* parent__, Color color__, FieldCoords position__)
      : MovingObject(parent__)
      , color_(color__)
  {
    placeAt(FloatFieldCoords(position__));
  }

  Color color() const {
    return color_;
  }

  Bonus bonus() const {
    return bonus_;
  }

  AutoStoppingEffectType<FadingEffectType>& bonusImage() {
    return bonusImage_;
  }

  void setBonus(Bonus bonus__) {
    bonus_ = bonus__;
  }

private:
  Color color_;
  Bonus bonus_ = Bonus::None;
  AutoStoppingEffectType<FadingEffectType> bonusImage_;
};


struct DisappearingLine {
  Color blockColor[FIELD_WIDTH];
  int row;
  Time startTime;
  Time duration;

  // TODO(Andrei): declare constants (?)
  float progress(Time currentTime) const {
    return math::bound(0.0f, (currentTime - startTime) / duration, 1.0f);
  }
};

}  // namespace engine
