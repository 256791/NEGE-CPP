#include "MovementController.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
constexpr double pi = 3.14159265358979323846;

template <class TransformationType>
MovementController<TransformationType>::MovementController(int width, int height, float fov, float near, float far)
    : Camera<TransformationType>(width, height, fov, near, far) {}

template <class TransformationType>
bool MovementController<TransformationType>::handle(Engine *engine)
{
  update(engine->getWindow());
  return false;
}

template <class TransformationType>
void MovementController<TransformationType>::update(GLFWwindow *window)
{
  if (!enabled)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    return;
  }
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  // todo change frame related speed to time related speed
  glm::vec3 translation = glm::vec3(0);
  if (glfwGetKey(window, config.forward) == GLFW_PRESS)
    translation += speed * glm::vec3(0, 0, -1);
  if (glfwGetKey(window, config.backward) == GLFW_PRESS)
    translation += speed * glm::vec3(0, 0, 1);
  if (glfwGetKey(window, config.right) == GLFW_PRESS)
    translation += speed * glm::vec3(1, 0, 0);
  if (glfwGetKey(window, config.left) == GLFW_PRESS)
    translation += speed * glm::vec3(-1, 0, 0);
  if (glfwGetKey(window, config.up) == GLFW_PRESS)
    translation += speed * glm::vec3(0, 1, 0);
  if (glfwGetKey(window, config.down) == GLFW_PRESS)
    translation += speed * glm::vec3(0, -1, 0);

  double x;
  double y;
  glfwGetCursorPos(window, &x, &y);
  glfwSetCursorPos(window, (this->width / 2), (this->height / 2));

  float ry = sensitivity * (y - (this->height / 2.f)) / this->height;
  float rx = sensitivity * (x - (this->width / 2.f)) / this->width;

  updatePosition(translation, rx, ry);
}

template <class TransformationType>
void MovementController<TransformationType>::updatePosition(glm::vec3 translation, float dyaw, float dpitch)
{
  if (translation != glm::vec3(0))
  {
    translation = glm::rotate(translation, this->yaw, glm::vec3(0, 1, 0));
    *this->transformation += translation;
  }

  this->pitch -= dpitch;
  this->yaw -= dyaw;

  if (this->pitch > glm::half_pi<float>())
    this->pitch = glm::half_pi<float>();
  if (this->pitch < -glm::half_pi<float>())
    this->pitch = -glm::half_pi<float>();

  if (this->yaw > 2 * glm::pi<float>())
    this->yaw -= 2 * glm::pi<float>();
  if (this->yaw < -2 * glm::pi<float>())
    this->yaw += 2 * glm::pi<float>();
}

template <class TransformationType>
void MovementController<TransformationType>::enable()
{
  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  enabled = true;
}

template <class TransformationType>
void MovementController<TransformationType>::disable()
{
  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  enabled = false;
}


MAKE_TRANSFORMATION_TEMPLATE_TYPES(MovementController)