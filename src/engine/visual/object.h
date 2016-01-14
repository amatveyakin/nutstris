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

  const VisualObject* parent() const {
    return parent_;
  }

  virtual FloatFieldCoords relativePosition(Time currentTime) const = 0;

  FloatFieldCoords absolutePosition(Time currentTime) const {
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

  FloatFieldCoords relativePosition(Time /*currentTime*/) const override {
    return position_;
  }

protected:
  mutable FloatFieldCoords position_;  // TODO(Andrei): remove "mutable"
};


class MovingObject : public AffixmentPointObject
{
public:
  using AffixmentPointObject::AffixmentPointObject;

  mutable std::vector<Motion> motions;  // TODO(Andrei): remove "mutable"

  void addMotion(FloatFieldCoords aimingShiftVector, Time movingStartTime, Time movingDuration) {
    // TODO(Andrei): Choose correct easing for each case
    motions.push_back(Motion(&math::easeInOutQuad, aimingShiftVector, movingStartTime, movingDuration));
  }

  virtual void placeAt(FloatFieldCoords newPosition) {
    AffixmentPointObject::placeAt(newPosition);
    motions.clear();
  }

  virtual FloatFieldCoords relativePosition(Time currentTime) const {
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
  virtual FloatFieldCoords relativePosition(Time currentTime) const {
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
  mutable Time lastUpdated_;  // TODO(Andrei): remove "mutable"
};

}  // namespace engine
