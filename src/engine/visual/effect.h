// TODO(Andrei): consider using composition based on virtual functions instead of templates

#pragma once

#include <list>

#include "engine/defs.h"
#include "engine/visual/object.h"


namespace engine {

// Base effect type, do not use directly
class BaseEffectType
{
protected:
  bool active_ = false;
};


class SmoothEffectType : public BaseEffectType
{
public:
  bool fullyInactive() const {
    return (progress_ == MIN_PROGRESS);
  }

  bool fullyActive() const {
    return (progress_ == MAX_PROGRESS);
  }

  bool somehowActive() const {
    return (progress_ > MIN_PROGRESS);
  }

protected:
  // Remove these constants (?)
  static constexpr float MIN_PROGRESS = 0.0f;
  static constexpr float MAX_PROGRESS = 1.0f;
  static constexpr float PROGRESS_RANGE = MAX_PROGRESS - MIN_PROGRESS;

  float progress_ = MIN_PROGRESS;
  Time lastUpdated_ = 0.0s;
};


// An effect that repeats periodically (starting from zero moment)
class PeriodicalEffectType : public SmoothEffectType
{
public:
  void enable(Time newPeriod) {
    active_ = true;
    isStopping_ = false;
    period_ = newPeriod;
  }

  void disable() {
    isStopping_ = true;
    stoppingAcceleration_ = 0.0s;
  }

  float progress(Time currentTime) {
    if (!active_) {
      lastUpdated_ = currentTime;
      return MIN_PROGRESS;
    }

    float progress_increment = (currentTime - lastUpdated_) / period_;
    progress_ += progress_increment;
    if (isStopping_) {
      stoppingAcceleration_ += progress_increment * STOPPING_ACCELERATION_COEFF;
      progress_ += stoppingAcceleration_ / 1.0s;
    }

    while (progress_ > MAX_PROGRESS) {
      if (isStopping_) {
        progress_ = MIN_PROGRESS;
        active_ = false;
      }
      else
        progress_ -= PROGRESS_RANGE;
    }
    lastUpdated_ = currentTime;
    return progress_;
  }

protected:
  static constexpr Time STOPPING_ACCELERATION_COEFF = 0.05s;

  Time period_ = 1.0s;
  bool isStopping_ = false;
  Time stoppingAcceleration_ = 0.0s;
};


// An effect that can fade in and (or) out. If (active == true) the effect is turned on
// and thus is either functioning normally [if (current_time > endTime] or is being
// activated now [if (current time <= endTime)]. If (active == false) everything is vice versa.
class FadingEffectType : public SmoothEffectType
{
public:
  void enable(Time newDuration) {
    active_ = true;
    duration_ = newDuration;
  }

  void disable() {
    active_ = false;
  }

  float progress(Time currentTime) {
    float progressChange = (currentTime - lastUpdated_) / duration_;
    if (active_)
      progress_ = std::min(progress_ + progressChange, MAX_PROGRESS);
    else
      progress_ = std::max(progress_ - progressChange, MIN_PROGRESS);
    lastUpdated_ = currentTime;
    return progress_;
  }

protected:
  Time duration_ = 1.0s;
};


// Effect that acts once
class SingleEffectType : public SmoothEffectType   // Name (?)
{
public:
  void enable(Time newDuration) {
    active_ = true;
    duration_ = newDuration;
  }

  // TODO(Andrei): Should SingleEffectType::disable be simply ignored?
  void disable() { }

  float progress(Time currentTime) {
    float progressChange = (currentTime - lastUpdated_) / duration_;
    if (active_) {
      progress_ += progressChange;
      if (progress_ > MAX_PROGRESS)
      {
        progress_ = MIN_PROGRESS;
        active_ = false;
      }
    }
    lastUpdated_ = currentTime;
    return progress_;
  }

protected:
  Time duration_ = 1.0s;
};


// Effect that fades in and than immediately fades out.
class FlashEffectType : public SmoothEffectType
{
public:
  void enable(Time newDuration) {
    active_ = true;
    halfDuration_ = newDuration / 2.0f;
  }

  void disable() {
    active_ = false;
  }

  float progress(Time currentTime) {
    float progressChange = (currentTime - lastUpdated_) / halfDuration_;
    if (active_) {
      progress_ += progressChange;
      if (progress_ > MAX_PROGRESS)
      {
        progress_ = MAX_PROGRESS;
        active_ = false;
      }
    }
    else
      progress_ = std::max(progress_ - progressChange, MIN_PROGRESS);
    lastUpdated_ = currentTime;
    return progress_;
  }

protected:
  Time halfDuration_ = 1.0s;
};


// TODO(Andrei): Join with FadingEffectType (?)

// Effect fades in and remains fully active until it is reset
class PermanentEffectType : public SmoothEffectType   // Name (?)
{
public:
  void enable(Time newDuration) {
    active_ = true;
    duration_ = newDuration;
  }

  void disable() {
    active_ = false;
    progress_ = MIN_PROGRESS;
  }

  /*void restart(Time newDuration)
  {
    disable();
    enable(newDuration);
  }*/

  float progress(Time currentTime) {
    float progressChange = (currentTime - lastUpdated_) / duration_;
    if (active_) {
      progress_ += progressChange;
      if (progress_ > MAX_PROGRESS)
        progress_ = MAX_PROGRESS;
    }
    lastUpdated_ = currentTime;
    return progress_;
  }

protected:
  Time duration_ = 1.0s;
};


template<class EffectT>
class AutoStartingEffectType : public EffectT
{
public:
  void startAt(Time startTime) {
    startTime_ = startTime;
  }

  void enable(Time newDuration) {
    startTime_ = WILL_NEVER_HAPPEN;
    EffectT::enable();
  }

  float progress(Time currentTime) {
    if (currentTime > startTime_)
      enable();
    return EffectT::progress(currentTime);
  }

protected:
  Time startTime_ = WILL_NEVER_HAPPEN;
};


template<class EffectT>
class AutoStoppingEffectType : public EffectT
{
public:
  void stopAt(Time stopTime) {
    stopTime_ = stopTime;
  }

  void disable() {
    stopTime_ = WILL_NEVER_HAPPEN;
    EffectT::disable();
  }

  float progress(Time currentTime) {
    if (currentTime > stopTime_)
      disable();
    return EffectT::progress(currentTime);
  }

protected:
  Time stopTime_ = WILL_NEVER_HAPPEN;
};


struct DirectedEffectExtraType
{
  int sender = 0;
  int target = 0;
};


using HintEffect = PermanentEffectType;

using HintMaterializationEffect = PermanentEffectType;

using FieldCleaningEffect = SingleEffectType; // --> FlashEffectType (?)

// if can't actually fade out :-)
using PlayerDyingEffect = FadingEffectType;

using NoHintEffect = FadingEffectType;

using FlippedScreenEffect = FadingEffectType;

using RotatingFieldEffect = PeriodicalEffectType;

using SemicubesEffect = FadingEffectType;

using WaveEffect = PeriodicalEffectType; // += FadingEffectType (?)

class LanternEffect : public FadingEffectType, public MagnetObject { };

class PieceTheftEffect : public SingleEffectType, public DirectedEffectExtraType { };


class PlayerVisualEffects
{
public:
  HintEffect hint;
  HintMaterializationEffect hintMaterialization;
  FieldCleaningEffect fieldCleaning;
  PlayerDyingEffect playerDying;
  NoHintEffect noHint;
  FlippedScreenEffect flippedScreen;
  RotatingFieldEffect rotatingField;
  SemicubesEffect semicubes;
  WaveEffect wave;
  LanternEffect lantern;
  PieceTheftEffect* pieceTheftPtr = nullptr;
};


class GlobalVisualEffects
{
public:
  std::list<PieceTheftEffect> pieceThefts;
};

}  // namespace engine
