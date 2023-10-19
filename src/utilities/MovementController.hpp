#ifndef MOVEMENT_CONTROLLER
#define MOVEMENT_CONTROLLER

#include <GLFW/glfw3.h>
#include "../components/Camera.hpp"
#include "../Event.hpp"

template <class TransformationType>
class MovementController : public Camera<TransformationType>, public Event
{
public:
  MovementController(int width, int height, float fov = 45.f, float near = 0.001f, float far = 100.f);

  bool handle(Engine *engine) override;
  virtual void update(GLFWwindow* window);
  virtual void updatePosition(glm::vec3 translation, float dyaw, float dpitch);
  void enable();
  void disable();

  struct KeyConfig{
    int forward = GLFW_KEY_W;
    int left = GLFW_KEY_A;
    int backward = GLFW_KEY_S;
    int right = GLFW_KEY_D;
    int up = GLFW_KEY_SPACE;
    int down = GLFW_KEY_LEFT_CONTROL;
  };
private:
  KeyConfig config;
  float speed = 0.01f;
	float sensitivity = 1.f;
  bool enabled = false;
};
#endif