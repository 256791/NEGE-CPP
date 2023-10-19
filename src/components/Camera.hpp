#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include "Transformation.hpp"

template <class TransformationType>
class Camera
{
public:
  Camera(int width, int height, float fov = 45.f, float near = 0.001f, float far = 100.f);
  virtual ~Camera();

  glm::mat4 getProjectionMatrix();
  TransformationType* getViewTransform();
  TransformationType* getCameraTransform();
  glm::vec3 getCameraPosition();

protected:
  int width;
  int height;
public:
  float pitch;
  float yaw;
  TransformationType* transformation;

private:
  TransformationType* view;
  glm::mat4 projectionMatrix;
};
#endif