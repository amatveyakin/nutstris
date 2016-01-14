#pragma once

#include <list>

#include "engine/defs.h"
#include "engine/visual/object.h"


namespace engine {

class BaseEffectType {
public:
  virtual ~BaseEffectType();

  virtual double progress(Time currentTime) const = 0;

  virtual void enable(Time currentTime) {
    if (!enabled_) {
      enabled_ = true;
      lastToggled_ = currentTime;
    }
  }

  virtual void disable(Time currentTime) {
    if (enabled_) {
      initialProgress_ = progress(currentTime);
      enabled_ = false;
      lastToggled_ = currentTime;
    }
  }

  virtual void reset(Time currentTime) {
    initialProgress_ = kMinProgress;
    enabled_ = false;
    lastToggled_ = currentTime;
  }

protected:
  // TODO: Remove (?) It is implicitly assumed that these constants equal these value in some places.
  static constexpr double kMinProgress = 0.0;
  static constexpr double kMaxProgress = 1.0;
  static constexpr double kProgressRange = kMaxProgress - kMinProgress;

  static constexpr double fmodBackAndForth(double dividend, double divisor) {
    double v = std::fmod(dividend, 2 * divisor);
    if (v < divisor)
      return v;
    else
      return 2 * divisor - v;
  }

  bool enabled_ = false;
  double initialProgress_ = kMinProgress;
  Time lastToggled_ = 0.0s;
};


// An effect that repeats periodically
class PeriodicalEffectType : public BaseEffectType {
  using Parent = BaseEffectType;
public:
  PeriodicalEffectType(Time period__)
    : period_(period__) {}

  void enable(Time currentTime) override {
    Parent::enable(currentTime);
    stoppingSince_ = Time::min();
  }

  void disable(Time currentTime) override {
    if (enabled_)
      stoppingSince_ = currentTime;
    Parent::disable(currentTime);
  }

  double progress(Time currentTime) const {
    if (enabled_) {
      double progressDelta = (currentTime - lastToggled_) / period_;
      return fmod(initialProgress_ + progressDelta, kProgressRange);
    } else {
      // These tricks are requires, because we don't have Time^2 unit
      double progressDelta = math::sqr((currentTime - lastToggled_) / 1.0s) * 1.0s / period_ * 1.0s * kStoppingSpeed;
      return std::min(initialProgress_ + progressDelta, kMaxProgress);
    }
  }

protected:
  static constexpr Speed kStoppingSpeed = 1.0 / 0.05s;

  Time period_ = 0.0s;
  Time stoppingSince_ = Time::min();
};

// Fades in on enable, fades out on disable.
class NormalEffectType : public BaseEffectType {
public:
  NormalEffectType(Time duration__)
    : duration_(duration__) {}

  void setDuration(Time duration__) {
    duration_ = duration__;
  }

  double progress(Time currentTime) const {
    double progressDelta = (currentTime - lastToggled_) / duration_;
    if (enabled_)
      return std::min(initialProgress_ + progressDelta, kMaxProgress);
    else
      return std::max(initialProgress_ - progressDelta, kMinProgress);
  }

protected:
  Time duration_ = 0.0s;
};

// Effect that fades in and than immediately fades out.
class FlashEffectType : public BaseEffectType {
public:
  FlashEffectType(Time halfDuration__)
    : halfDuration_(halfDuration__) {}

  double progress(Time currentTime) const {
    double progressDelta = (currentTime - lastToggled_) / halfDuration_;
    if (enabled_) {
      return fmodBackAndForth(initialProgress_ + progressDelta, kProgressRange);
    } else {
      return std::max(initialProgress_ - progressDelta, kMinProgress);
    }
  }

protected:
  Time halfDuration_ = 0.0s;
};


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
