#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>

#include "../Engine.hpp"
#include "../Event.hpp"

class Console : public Event
{
public:
  Console(Engine* engine, std::istream &in, std::ostream &out);
  void start();
  void join();
  void stop();
  bool handle(Engine* engine) override;

private:
  std::istream &in;
  std::ostream &out;
  Engine* engine;
  bool started;
  bool running;
  std::thread self;

  std::mutex eventQueueGuard;
  std::queue<std::function<void(Engine* engine)>> eventQueue;

  void run();
};
#endif