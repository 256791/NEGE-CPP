#include "Gyrovector.hpp"
#include <iostream>

// glm::dvec3 mobiusAddition(glm::dvec3 a, glm::dvec3 b)
// {
//   glm::dvec3 c = -glm::cross(a, b);
//   double d = 1.0 + glm::dot(a, b);
//   glm::dvec3 t = a + b;
//   return (t * d + glm::cross(c, t)) / (d * d + glm::dot(c, c));
// }

glm::dvec3 Gyrovector::mobiusAddition(glm::dvec3 u, glm::dvec3 v)
{
  return ((1 + 2 * glm::dot(u, v) + glm::dot(v, v)) * u + (1 - glm::dot(u, u)) * v) /
         (1 + 2 * glm::dot(u, v) + glm::dot(u, u) * glm::dot(v, v));
}
// glm::dvec3 einsteinAddition(const glm::dvec3 &u, const glm::dvec3 &v)
// {
//   double gammaI = sqrt(1.f - glm::dot(u, u));
//   double gamma = 1.f / gammaI;
//   // return 1.f / (1.f + glm::dot(u, v)) * (u + (gammaI)*v + (gamma / (1.f + gamma)) * glm::dot(u, v) * u);
//   return 1.f / (1.f + glm::dot(u, v)) * ((gammaI)*v + u * (1 + (gamma / (1.f + gamma)) * glm::dot(u, v)));
// }

glm::dquat getGyration(const glm::dvec3 &u, const glm::dvec3 &v)
{
  glm::dquat a = glm::angleAxis(glm::radians(180.0), glm::normalize(u));
  glm::dquat b = glm::angleAxis(glm::radians(180.0), glm::normalize(v));
  return glm::normalize(a * b);
}
// glm::dquat mobiusGyration(const glm::dvec3 &u, const glm::dvec3 &v)
// {
//   glm::dquat a = glm::angleAxis(glm::radians(180.0), glm::normalize(mobiusAddition(u, v)));
//   glm::dquat b = glm::angleAxis(glm::radians(180.0), glm::normalize(u + v));
//   return glm::normalize(a * b);
//   // glm::dvec3 c = -glm::cross(u, v);
//   // double d = 1.0f + glm::dot(u, v);
//   // glm::dquat q = glm::dquat(c.x, c.y, c.z, d);
//   // return glm::normalize(q);
// }


Gyrovector::Gyrovector()
    : position(0), gyration(1.0, 0.0, 0.0, 0.0) {}

Gyrovector::Gyrovector(const Gyrovector &copy)
    : Gyrovector(copy.position, copy.gyration) {}

Gyrovector::Gyrovector(const glm::dvec3 &position, const glm::dquat &gyration)
    : position(position), gyration(gyration) {}

Gyrovector Gyrovector::operator+(const glm::dvec3 &rhs)
{
  glm::dvec3 v = rhs;
  v = v * glm::dquat(gyration.w, -gyration.x, -gyration.y, -gyration.z);
  glm::dvec3 u = mobiusAddition(position, v);
  return Gyrovector(v, gyration * getGyration(u, position + v));
}

Gyrovector Gyrovector::operator*(const glm::dquat &rhs)
{
  return Gyrovector(position, gyration * rhs);
}

Gyrovector Gyrovector::operator+=(const glm::dvec3 &rhs)
{
  glm::dvec3 v = rhs;
  v = v * glm::dquat(gyration.w, -gyration.x, -gyration.y, -gyration.z);

  glm::dvec3 u = mobiusAddition(position, v);
  gyration = gyration * getGyration(u, position + v);
  position = u;
  return *this;
}

Gyrovector Gyrovector::operator*=(const glm::dquat &rhs)
{
  gyration = gyration * rhs;
  return *this;
}