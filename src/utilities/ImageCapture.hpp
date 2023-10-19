#ifndef IMAGE_HPP
#define IMAGE_HPP
#include <string>
#include <thread>

#include "../Event.hpp"

class ImageCaptutre : public Event
{
public:
  enum Mode
  {
    SINGLE_IMAGE,
    CONTINOUS,
    INTERVAL
  };
  ImageCaptutre(std::string filename, Mode mode, int interval = 0);
  ~ImageCaptutre();

  bool handle(Engine *engine) override;

private:
  std::string filename;
  Mode mode;
  int image;
  int counter;
  int interval;

  std::thread worker;
  bool isRunning;
  
};
#endif