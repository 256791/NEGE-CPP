#include "Engine.hpp"
#include <stdexcept>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>
// #include <glm/gtc/matrix_transform.hpp>
#include <iostream>

GLenum _glCheckError(const char *file, int line)
{
  GLenum errorCode;
  while ((errorCode = glGetError()) != GL_NO_ERROR)
  {
    std::string error;
    switch (errorCode)
    {
    case GL_INVALID_ENUM:
      error = "INVALID_ENUM";
      break;
    case GL_INVALID_VALUE:
      error = "INVALID_VALUE";
      break;
    case GL_INVALID_OPERATION:
      error = "INVALID_OPERATION";
      break;
    case GL_OUT_OF_MEMORY:
      error = "OUT_OF_MEMORY";
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      error = "INVALID_FRAMEBUFFER_OPERATION";
      break;
    }
    std::cerr << error << " | " << file << " (" << line << ")\n";
  }
  return errorCode;
}
#ifdef DEBUG
#define glCheckError() _glCheckError(__FILE__, __LINE__)
#else
#define glCheckError() ;
#endif

Engine::Engine(int windowWidth, int windowHeight, std::string windowName)
    : windowWidth(windowWidth), windowHeight(windowHeight), frame(0)
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

  glfwSwapInterval((int)(glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate / 30));

  gladLoadGL();

  e3 = new E3Shader();
  h3 = new H3Shader();
  h2e = new H2EShader();

  glViewport(0, 0, windowWidth, windowHeight);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Engine::renderFrame()
{
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // glLineWidth(2.f);
  if (E3Scene *scene = dynamic_cast<E3Scene *>(currentScene))
  {
    e3->use();
    e3->clear();
    glCheckError();
    glUniform3fv(e3->uCameraPosition, 1, glm::value_ptr(scene->camera->getCameraPosition()));
    glCheckError();
    scene->camera->getViewTransform()->bind(e3->uView);
    glCheckError();
    glUniformMatrix4fv(e3->uProjection, 1, GL_FALSE, glm::value_ptr(scene->camera->getProjectionMatrix()));
    glCheckError();
    scene->lights.bind(e3->uLight);
    for (Entity<E3Transformation> entity : scene->entities)
    {
      entity.transformation->bind(e3->uModel);
      glCheckError();
      entity.draw(e3->uCurrentMaterial);
      glCheckError();
    }
  }
  else if (TypedScene<H3LockedTransformation> *scene = dynamic_cast<TypedScene<H3LockedTransformation> *>(currentScene))
  {
    h3->use();
    h3->clear();
    glUniform3fv(h3->uCameraPosition, 1, glm::value_ptr(scene->camera->getCameraPosition()));
    scene->camera->getViewTransform()->bind(h3->uView);
    glUniformMatrix4fv(h3->uProjection, 1, GL_FALSE, glm::value_ptr(scene->camera->getProjectionMatrix()));
    scene->lights.bind(h3->uLight);
    glCheckError();
    for (Entity<H3LockedTransformation> entity : scene->entities)
    {
      entity.transformation->bind(h3->uModel);
      glCheckError();
      entity.draw(h3->uCurrentMaterial);
      glCheckError();
    }
  }
  else if (H3Scene *scene = dynamic_cast<H3Scene *>(currentScene))
  {
    h3->use();
    h3->clear();
    glUniform3fv(h3->uCameraPosition, 1, glm::value_ptr(scene->camera->getCameraPosition()));
    scene->camera->getViewTransform()->bind(h3->uView);
    glUniformMatrix4fv(h3->uProjection, 1, GL_FALSE, glm::value_ptr(scene->camera->getProjectionMatrix()));
    glCheckError();
    scene->lights.bind(h3->uLight);
    for (Entity<H3Transformation> entity : scene->entities)
    {
      entity.transformation->bind(h3->uModel);
      glCheckError();
      entity.draw(h3->uCurrentMaterial);
      glCheckError();
    }
  }
  else if (H2EScene *scene = dynamic_cast<H2EScene *>(currentScene))
  {
    h2e->use();
    h2e->clear();
    glUniform3fv(h2e->uCameraPosition, 1, glm::value_ptr(scene->camera->getCameraPosition()));
    glCheckError();
    scene->camera->getViewTransform()->bind(h2e->uView);
    glCheckError();

    glUniformMatrix4fv(h2e->uProjection, 1, GL_FALSE, glm::value_ptr(scene->camera->getProjectionMatrix()));
    glCheckError();
    scene->lights.bind(h2e->uLight);
    for (Entity<H2ETransformation> entity : scene->entities)
    {
      entity.transformation->bind(h2e->uModel);
      glCheckError();
      entity.draw(h2e->uCurrentMaterial);
      glCheckError();
    }
  }
  frame++;
  glCheckError();
  glfwSwapBuffers(window);
}

void Engine::handleEvents()
{
  glfwPollEvents();
  std::vector<Event *>::iterator event = events.begin();
  while (event != events.end())
  {
    if ((*event)->handle(this))
      events.erase(event);
    else
      event++;
  }
}

void Engine::loop()
{
  while (!glfwWindowShouldClose(window))
  {
    renderFrame();
    handleEvents();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      break;
  }
}

void Engine::changeScene(Scene *scene)
{
  currentScene = scene;
}

void Engine::addEvent(Event *event)
{
  events.push_back(event);
}

GLFWwindow *Engine::getWindow() { return window; }
unsigned long long Engine::getCurrentFrame() { return frame; }

Engine::~Engine()
{
  delete e3;
  delete h3;
  glfwDestroyWindow(window);
  glfwTerminate();
}