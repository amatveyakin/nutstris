#include "effect.h"


namespace engine {

constexpr double BaseEffectType::kMinProgress;
constexpr double BaseEffectType::kMaxProgress;
constexpr double BaseEffectType::kProgressRange;

constexpr Speed PeriodicalEffectType::kStoppingSpeed;


BaseEffectType::~BaseEffectType() {}

}  // namespace engine
