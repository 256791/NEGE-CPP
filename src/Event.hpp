#ifndef EVENT_HPP
#define EVENT_HPP

#include <GLFW/glfw3.h>
#include <functional>
#include "Engine.hpp"

class Engine;
class Event
{
public:
  virtual bool handle(Engine *engine) = 0;
};

template <class EventData>
class FunctionalEvent : public Event
{
public:
  FunctionalEvent(std::function<bool(Engine *engine, EventData pdata)> &f, EventData data)
      : data(data), f(f) {}
      
  bool handle(Engine *engine) override
  {
    return f(engine, data);
  }

private:
  EventData data;
  std::function<bool(Engine *engine, EventData pdata)> &f;
};

#endif