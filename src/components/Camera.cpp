#include "Camera.hpp"
#include <glm/ext.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

template <class TransformationType>
Camera<TransformationType>::Camera(int width, int height, float fov, float near, float far)
    : width(width), height(height), pitch(0), yaw(0)
{
  projectionMatrix = glm::perspective(glm::radians(fov), (width / (float)height), near, far);
  transformation = new TransformationType();
  view = new TransformationType();
}

template <class TransformationType>
glm::mat4 Camera<TransformationType>::getProjectionMatrix()
{
  return projectionMatrix;
}

template <class TransformationType>
TransformationType *Camera<TransformationType>::getViewTransform()
{
  *view = *transformation;
  *view *= glm::angleAxis(yaw, glm::vec3(0, 1, 0));
  *view *= glm::angleAxis(pitch, glm::vec3(1, 0, 0));
  view->invert();
  return view;
}

template <class TransformationType>
TransformationType *Camera<TransformationType>::getCameraTransform()
{
  return transformation;
}
template <class TransformationType>
glm::vec3 Camera<TransformationType>::getCameraPosition()
{
  return transformation->getPosition();
}

template <class TransformationType>
Camera<TransformationType>::~Camera()
{
  delete transformation;
  delete view; 
}


MAKE_TRANSFORMATION_TEMPLATE_TYPES(Camera)