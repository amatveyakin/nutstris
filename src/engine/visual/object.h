#pragma once

#include <vector>

#include "engine/defs.h"
#include "engine/visual/motion.h"
#include "math/metric.h"


namespace engine {

class VisualObject
{
public:
  VisualObject(VisualObject* parent__ = nullptr)
      : parent_(parent__) {}

  virtual ~VisualObject() {}

  VisualObject* parent() {
    return parent_;
  }

  virtual FloatFieldCoords relativePosition(Time currentTime) = 0;

  FloatFieldCoords absolutePosition(Time currentTime) {
    if (parent_)
      return parent_->absolutePosition(currentTime) + relativePosition(currentTime);
    else
      return relativePosition(currentTime);
  }

private:
  VisualObject* parent_ = nullptr;
};


class AffixmentPointObject : public VisualObject  // name -> (?)
{
public:
  using VisualObject::VisualObject;

  virtual void placeAt(FloatFieldCoords newPosition) {
    position_ = newPosition;
  }

  FloatFieldCoords relativePosition(Time /*currentTime*/) override {
    return position_;
  }

protected:
  FloatFieldCoords position_;
};


class MovingObject : public AffixmentPointObject
{
public:
  using AffixmentPointObject::AffixmentPointObject;

  std::vector<Motion> motions;

  void addMotion(FloatFieldCoords aimingShiftVector, Time movingStartTime, Time movingDuration) {
    motions.push_back(Motion(&math::easeInOutQuad, aimingShiftVector, movingStartTime, movingDuration));
  }

  virtual void placeAt(FloatFieldCoords newPosition) {
    AffixmentPointObject::placeAt(newPosition);
    motions.clear();
  }

  virtual FloatFieldCoords relativePosition(Time currentTime) {
    FloatFieldCoords currentPosition = position_;
    for (auto motionIt = motions.begin(); motionIt != motions.end(); ) {
      FloatFieldCoords motionShift = motionIt->shiftVector(currentTime);
      currentPosition += motionShift;
      if (motionIt->finished(currentTime)) {
        position_ += motionShift;
        motionIt = motions.erase(motionIt);
      }
      else
        ++motionIt;
    }
    return currentPosition;
  }
};


class MagnetObject : public AffixmentPointObject
{
public:
  VisualObject* binding() const {
    return binding_;
  }
  void bindTo(VisualObject* newBinding) {
    binding_ = newBinding;
  }
  void resetBinding() {
    binding_ = nullptr;
  }

  Speed maxSpeed() const {
    return maxSpeed_;
  }
  void setMaxSpeed(Speed newMaxSpeed) {
    maxSpeed_ = newMaxSpeed;
  }

  // TODO: update position separately, make getter constant
  virtual FloatFieldCoords relativePosition(Time currentTime) {
    Time deltaTime = currentTime - lastUpdated_;
    lastUpdated_ = currentTime;
    if (!binding_)
      return position_;
    FloatFieldCoords shiftVector = binding_->absolutePosition(currentTime) - position_;
    double deltaDistance = math::L2::norm(shiftVector);
    double maxDistance = maxSpeed_.operator*(deltaTime);  // TODO: Fix. Note: ``maxSpeed_ * deltaTime'' doesn't compile in MSVC 2015
    FloatFieldCoords newPosition;
    if (deltaDistance <= maxDistance)
      newPosition = binding_->absolutePosition(currentTime);
    else
      newPosition = position_ + shiftVector * maxDistance / deltaDistance;  // (?)
    position_ = newPosition;
    return newPosition;
  }

protected:
  VisualObject* binding_ = nullptr;
  Speed maxSpeed_ = Speed::zero();
  Time lastUpdated_;
};

}  // namespace engine
