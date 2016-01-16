#pragma once

#include <list>

#include "engine/visual/effect.h"
#include "engine/visual/object.h"


namespace engine {

struct DirectedEffectExtraType
{
  int sender = 0;
  int target = 0;
};

using HintAppearanceEffect = NormalEffectType;
using HintMaterializationEffect = NormalEffectType;
using FieldCleaningEffect = NormalEffectType;
using PlayerDyingEffect = NormalEffectType;
using NoHintEffect = NormalEffectType;
using FlippedScreenEffect = NormalEffectType;
using RotatingFieldEffect = PeriodicalEffectType;
using SemicubesEffect = NormalEffectType;
using WaveEffect = PeriodicalEffectType;
using LanternEffect = NormalEffectType;
class PieceTheftEffect : public NormalEffectType, public DirectedEffectExtraType {
public:
  PieceTheftEffect();
};

class PlayerVisualEffects {
public:
  HintAppearanceEffect hintAppearance;
  HintMaterializationEffect hintMaterialization;
  FieldCleaningEffect fieldCleaning;
  PlayerDyingEffect playerDying;
  NoHintEffect noHint;
  FlippedScreenEffect flippedScreen;
  RotatingFieldEffect rotatingField;
  SemicubesEffect semicubes;
  WaveEffect wave;
  LanternEffect lantern;
  MagnetObject lanternObject;
  PieceTheftEffect* pieceTheftPtr = nullptr;

  PlayerVisualEffects();
};

class GlobalVisualEffects
{
public:
  std::list<PieceTheftEffect> pieceThefts;
};

}  // namespace engine
