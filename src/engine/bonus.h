#pragma once

#include "util/containers.h"


namespace engine {

enum class Bonus {
  // no bonus
  None,

  // buffs
  EnlargeHintQueue,
  PieceTheft,

  // kind sorceries
  Heal,
  SlowDown,
  ClearField,

  // debuffs
  FlippedScreen,
  RotatingScreen,
  Wave,
  Lantern,
  CrazyPieces,
  TruncatedBlocks,
  NoHint,

  // evil sorceries
  SpeedUp,
//  FlipField,
};

const Bonus  FIRST_BONUS = Bonus::None;
const Bonus  LAST_BONUS = Bonus::SpeedUp;

bool isPermanent(Bonus bonus);
bool isKind(Bonus bonus);
bool isEvil(Bonus bonus);
bool isBuff(Bonus bonus);
bool isDebuff(Bonus bonus);
int bonusFrequency(Bonus bonus);

class ForEachBonus {
public:
  class BonusIterator {
  public:
    explicit BonusIterator(Bonus bonus__) : bonus_(bonus__) {}
    BonusIterator& operator++() {
      bonus_ = static_cast<Bonus>(static_cast<int>(bonus_) + 1);
      return *this;
    }
    Bonus operator*() const                   { return bonus_; }
    bool operator==(BonusIterator rhs) const  { return bonus_ == rhs.bonus_; }
    bool operator!=(BonusIterator rhs) const  { return bonus_ != rhs.bonus_; }
  private:
    Bonus bonus_;
  };

  BonusIterator begin() const     { return BonusIterator(FIRST_BONUS); }
  BonusIterator end() const       { return BonusIterator(LAST_BONUS); }
};

typedef util::StronglyTypedBitSet<Bonus, FIRST_BONUS, LAST_BONUS> BonusesBitSet;

}  // namespace engine
