#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Engine
{
public:
  Engine(int windowWidth, int windowHeight, std::string windowName);
  ~Engine();
  void loop();

private:
  GLFWwindow *window;
};
#endif