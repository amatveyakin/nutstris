#pragma once

#include <map>

#include "engine/defs.h"


namespace engine {

const Time EVENT_DELAY_TIME = 0.01s;  // TODO(Andrei): consider delaying by 0s (checking next cycle)

enum EventType
{
  etPieceLowering,
  etLineCollapse,
  etNewPiece,
  etRoutineSpeedUp,
  etBonusAppearance,
  etBonusDisappearance,
  etHeal,
  etBeginClearField,
  etEndClearField,
  etKill
};


struct LineCollapseParameters
{
  int row;
};

union EventParameters
{
  LineCollapseParameters lineCollapse;
};


struct Event
{
  EventType type;
  Time activationTime;
  EventParameters parameters;

  Event() { }
  Event(EventType type__, Time activationTime__) : type(type__), activationTime(activationTime__) { }

  struct CompareByActivationTime {
    bool operator()(const Event& lhs, const Event& rhs) {
      return lhs.activationTime < rhs.activationTime;
    }
  };
};


// TODO(Andrei): (?) reorganize (may be add counter of events of a certain type or use several sets ot maps)
class EventSet
{
public:
  typedef std::multiset<Event>::iterator iterator;

  iterator begin() {
    return events_.begin();
  }

  iterator end() {
    return events_.end();
  }

  void push(const Event& event) {
    events_.insert(event);
  }

  void push(const EventType eventType, Time activationTime) {
    events_.insert(Event(eventType, activationTime));
  }

  void pushWithUniquenessCheck(const EventType eventType, Time activationTime) {
    for (iterator i = begin(); i != end(); ++i)
      assert(i->type != eventType);
    push(eventType, activationTime);
  }

  const Event& top() {
    return *events_.begin();
  }

  void pop() {
    events_.erase(events_.begin());
  }

  void erase(iterator it) {
    events_.erase(it);
  }

  void eraseEventType(EventType eventType) {
    for (iterator i = begin(); i != end(); ) {
      if (i->type == eventType)
        events_.erase(i++);
      else
        ++i;
    }
  }

  void clear() {
    events_.clear();
  }

  bool empty() {
    return events_.empty();
  }

private:
  std::multiset<Event, Event::CompareByActivationTime> events_;
};

}  // namespace engine
