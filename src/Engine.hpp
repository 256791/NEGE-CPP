#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Event.hpp"
#include "components/Scene.hpp"
#include "shaders/E3Shader.hpp"
#include "shaders/H3Shader.hpp"
#include "shaders/H2EShader.hpp"

class Event;
class Engine
{
public:
  Engine(int windowWidth, int windowHeight, std::string windowName);
  ~Engine();
  void loop();

  GLFWwindow* getWindow();
  void changeScene(Scene* scene);
  void addEvent(Event* event);
  void renderFrame();
  void handleEvents();
  unsigned long long getCurrentFrame();

  Scene* currentScene;
  std::vector<Event *> events;

  GLFWwindow *window;
  E3Shader *e3;
  H3Shader *h3;
  H2EShader *h2e;
  int windowWidth;
  int windowHeight;
  unsigned long long frame;
};
#endif