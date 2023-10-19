#ifndef GYROVECTOR_HPP
#define GYROVECTOR_HPP
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct Gyrovector
{
  Gyrovector();
  Gyrovector(const Gyrovector &copy);
  Gyrovector(const glm::dvec3 &position, const glm::dquat &gyration);

  Gyrovector operator+(const glm::dvec3 &rhs);
  Gyrovector operator*(const glm::dquat &rhs);

  Gyrovector operator+=(const glm::dvec3 &rhs);
  Gyrovector operator*=(const glm::dquat &rhs);

  glm::dvec3 position;
  glm::dquat gyration;
  
  static glm::dvec3 mobiusAddition(glm::dvec3 u, glm::dvec3 v);
};

struct Gyrovector2
{
  Gyrovector2();
  Gyrovector2(const Gyrovector2 &copy);
  Gyrovector2(const glm::dvec2 &position, const double &gyration);

  Gyrovector2 operator+(const glm::dvec2 &rhs);
  Gyrovector2 operator*(const double &rhs);

  Gyrovector2 operator+=(const glm::dvec2 &rhs);
  Gyrovector2 operator*=(const double &rhs);

  glm::dvec2 position;
  double gyration;
};

#endif