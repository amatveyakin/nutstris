#pragma once

#include <vector>

#include "engine/defs.h"
#include "engine/visual/motion.h"
#include "math/metric.h"


namespace engine {

class VisualObject {
public:
  VisualObject(VisualObject* parent__ = nullptr)
      : parent_(parent__) {}

  virtual ~VisualObject() {}

  const VisualObject* parent() const {
    return parent_;
  }

  virtual void update(Time currentTime) = 0;

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


class AffixmentPointObject : public VisualObject {  // name -> (?)
public:
  using VisualObject::VisualObject;

  virtual void placeAt(FloatFieldCoords newPosition) {
    position_ = newPosition;
  }

  FloatFieldCoords relativePosition(Time /*currentTime*/) const override final {
    return position_;
  }

protected:
  FloatFieldCoords position_;
};


class MovingObject : public AffixmentPointObject {
public:
  using AffixmentPointObject::AffixmentPointObject;

  void addMotion(FloatFieldCoords aimingShiftVector, Time movingStartTime, Time movingDuration) {
    // TODO(Andrei): Choose correct easing for each case
    motions.push_back(Motion(&math::easeInOutQuad, aimingShiftVector, movingStartTime, movingDuration));
  }

  void placeAt(FloatFieldCoords newPosition) override {
    AffixmentPointObject::placeAt(newPosition);
    placementPosition_ = newPosition;
    motions.clear();
  }

  void update(Time currentTime) override {
    position_ = placementPosition_;
    for (auto motionIt = motions.begin(); motionIt != motions.end(); ) {
      FloatFieldCoords motionShift = motionIt->shiftVector(currentTime);
      position_ += motionShift;
      if (motionIt->finished(currentTime)) {
        placementPosition_ += motionShift;
        motionIt = motions.erase(motionIt);
      }
      else
        ++motionIt;
    }
  }

protected:
  FloatFieldCoords placementPosition_;
  std::vector<Motion> motions;
};


class MagnetObject : public AffixmentPointObject {
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

  void update(Time currentTime) override {
    Time deltaTime = currentTime - lastUpdated_;
    lastUpdated_ = currentTime;
    if (!binding_)
      return;
    FloatFieldCoords shiftVector = binding_->absolutePosition(currentTime) - position_;
    double deltaDistance = math::L2::norm(shiftVector);
    double maxDistance = maxSpeed_.operator*(deltaTime);  // TODO: Fix. Note: ``maxSpeed_ * deltaTime'' doesn't compile in MSVC 2015
    if (deltaDistance <= maxDistance)
      position_ = binding_->absolutePosition(currentTime);
    else
      position_ += shiftVector * maxDistance / deltaDistance;
  }

protected:
  VisualObject* binding_ = nullptr;
  Speed maxSpeed_ = Speed::zero();
  Time lastUpdated_;
};

}  // namespace engine
