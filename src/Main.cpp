#include "Engine.hpp"
#include "shaders/E3Shader.hpp"

#include <iostream>

int main()
{
  Engine engine(1280, 720, "NEGE");
  engine.loop();
  return 0;
}