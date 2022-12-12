#include "Engine.hpp"
#include <stdexcept>

Engine::Engine(int windowWidth, int windowHeight, std::string windowName)
{
  if (!glfwInit())
    throw std::runtime_error("OpenGL initialization failed");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), NULL, NULL);

  if (!window)
  {
    glfwTerminate();
    throw std::runtime_error("Window initialization failed");
  }

  glfwMakeContextCurrent(window);

  // glfwSwapInterval((int)(glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate / FPS_LIMIT));

  gladLoadGL();
}

void Engine::loop()
{
  while (!glfwWindowShouldClose(window))
  {
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

Engine::~Engine()
{
  glfwDestroyWindow(window);
  glfwTerminate();
}