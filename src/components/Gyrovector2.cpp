#include "Gyrovector.hpp"
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

// direct implementation
glm::dvec2 einsteinAddition(const glm::dvec2 &u, const glm::dvec2 &v)
{
  double gammaI = sqrt(1.f - glm::dot(u, u));
  double gamma = 1.f / gammaI;
  return 1.f / (1.f + glm::dot(u, v)) * (u + (gammaI)*v + (gamma / (1.f + gamma)) * glm::dot(u, v) * u);
}

double einsteinGyration(const glm::dvec2 &u, const glm::dvec2 &v)
{
  glm::dvec2 a = glm::normalize(einsteinAddition(u, v));
  glm::dvec2 b = glm::normalize(u + v);
  double angle = glm::angle(a, b);
  if (std::isnan(angle))
  {
    // std::cout << "NAN\n";
    return 0;
  }
  // std::cout << angle << '\n';
  return angle;
}

Gyrovector2::Gyrovector2()
    : position(0), gyration(0.0) {}

Gyrovector2::Gyrovector2(const Gyrovector2 &copy)
    : Gyrovector2(copy.position, copy.gyration) {}

Gyrovector2::Gyrovector2(const glm::dvec2 &position, const double &gyration)
    : position(position), gyration(gyration) {}

Gyrovector2 Gyrovector2::operator+(const glm::dvec2 &rhs)
{
  glm::dvec2 v = rhs;
  v = rotate(v, -gyration);
  return Gyrovector2(einsteinAddition(position, v), gyration + einsteinGyration(position, v));
}

Gyrovector2 Gyrovector2::operator*(const double &rhs)
{
  return Gyrovector2(position, gyration + rhs);
}

Gyrovector2 Gyrovector2::operator+=(const glm::dvec2 &rhs)
{
  glm::dvec2 v = rhs;
  v = rotate(v, -gyration);
  gyration = gyration + einsteinGyration(position, v);
  position = einsteinAddition(position, v);
  return *this;
}

Gyrovector2 Gyrovector2::operator*=(const double &rhs)
{
  gyration = gyration + rhs;
  return *this;
}