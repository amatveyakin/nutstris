#include "bonus.h"

#include <map>
#include "units.h"


namespace engine {

namespace {
enum Lifetime {
  OneShot,
  Permanent,
};
enum Desirability {
  Kind,
  Evil,
};

struct BonusProperties {
  BonusProperties(Lifetime lifetime__, Desirability desirability__, int frequency__)
    : lifetime(lifetime__), desirability(desirability__), frequency(frequency__) {}
  Lifetime lifetime = {};
  Desirability desirability = {};
  int frequency = 0;
};

struct PropertiesMap {
  std::map<Bonus, BonusProperties> data;

  PropertiesMap() {
    addBonus(Bonus::None            , {Permanent, Kind, 0 });
    addBonus(Bonus::EnlargeHintQueue, {Permanent, Kind, 0 });
    addBonus(Bonus::PieceTheft      , {OneShot  , Kind, 0 });
    addBonus(Bonus::Heal            , {OneShot  , Kind, 10});
    addBonus(Bonus::SlowDown        , {OneShot  , Kind, 4 });
    addBonus(Bonus::ClearField      , {OneShot  , Kind, 2 });
    addBonus(Bonus::FlippedScreen   , {Permanent, Evil, 4 });
    addBonus(Bonus::RotatingScreen  , {Permanent, Evil, 3 });
    addBonus(Bonus::Wave            , {Permanent, Evil, 4 });
    addBonus(Bonus::Lantern         , {Permanent, Evil, 3 });
    addBonus(Bonus::CrazyPieces     , {Permanent, Evil, 0 });
    addBonus(Bonus::TruncatedBlocks , {Permanent, Evil, 4 });
    addBonus(Bonus::NoHint          , {Permanent, Evil, 0 });
    addBonus(Bonus::SpeedUp         , {OneShot  , Evil, 4 });
    const int kNumBonuses = static_cast<int>(LAST_BONUS) - static_cast<int>(FIRST_BONUS) + 1;
    assert(data.size() == kNumBonuses);
  }

  void addBonus(Bonus bonus, BonusProperties properties) {
    bool inserted = data.insert({bonus, properties}).second;
    assert(inserted);
  }
};

PropertiesMap propertiesMap;

const BonusProperties& bonusProperties(Bonus bonus) {
  auto it = propertiesMap.data.find(bonus);
  assert (it != propertiesMap.data.end());
  return it->second;
}
}  // namespace


bool isPermanent(Bonus bonus) {
  return bonusProperties(bonus).lifetime == Permanent;
}

bool isKind(Bonus bonus) {
  return bonusProperties(bonus).desirability == Kind;
}

bool isEvil(Bonus bonus) {
  return bonusProperties(bonus).desirability == Evil;
}

bool isBuff(Bonus bonus) {
  return isPermanent(bonus) && isKind(bonus);
}

bool isDebuff(Bonus bonus) {
  return isPermanent(bonus) && isEvil(bonus);
}

int bonusFrequency(Bonus bonus) {
  return bonusProperties(bonus).frequency;
}

}  // namespace engine
