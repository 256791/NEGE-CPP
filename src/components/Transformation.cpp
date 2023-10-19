#include "Transformation.hpp"
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/constants.hpp>
#include <string>
#include <iostream>

Transformation::Transformation() {}

E3Transformation::E3Transformation()
    : glm::mat4(1.0f) {}

E3Transformation::Location E3Transformation::uniformLocation(const GLuint program, const char *name)
{
  return glGetUniformLocation(program, name);
}

void E3Transformation::bind(E3Transformation::Location location)
{
  // glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transformation));
  // &(m[0].x)
  glm::mat4 m = *this;

  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m));
}

void E3Transformation::invert()
{
  glm::mat4::operator=(glm::inverse(*this));
}


E3Transformation &E3Transformation::operator*=(const glm::quat &rhs)
{
  glm::mat4 rotation = glm::mat4_cast(rhs);
  (*this) *= rotation;
  return *this;
}

E3Transformation &E3Transformation::operator*=(const glm::mat4 &rhs)
{
  glm::mat4::operator*=(rhs);
  return *this;
}

E3Transformation &E3Transformation::operator+=(const glm::vec3 &rhs)
{
  glm::mat4::operator=(glm::translate(*this, rhs));
  return *this;
}

void E3Transformation::operator=(const glm::mat4 &rhs)
{
  glm::mat4::operator=(rhs);
}

glm::vec3 E3Transformation::getPosition()
{
  return glm::vec3((*this)[3]);
}

void E3Transformation::scale(double scale)
{
  this->scaling.x *= scale;
  this->scaling.y *= scale;
  this->scaling.z *= scale;
  glm::mat4::operator*=(glm::scale(glm::identity<glm::mat4>(), glm::vec3(scale)));
}
void E3Transformation::scale(glm::vec3 scale)
{
  this->scaling *= scale;
  glm::mat4::operator*=(glm::scale(glm::identity<glm::mat4>(), scale));
}

////////////////////////////////////////


H3Transformation::H3Transformation()
    : Gyrovector(), scaling(1) {}

H3Transformation::Location H3Transformation::uniformLocation(const GLuint program, const char *name)
{
  H3Transformation::Location loc;
  loc.position = glGetUniformLocation(program, (std::string(name) + ".position").c_str());
  loc.gyration = glGetUniformLocation(program, (std::string(name) + ".gyration").c_str());
  loc.scale = glGetUniformLocation(program, (std::string(name) + ".scale").c_str());
  return loc;
}

void H3Transformation::bind(H3Transformation::Location location)
{
  glUniform3fv(location.position, 1, glm::value_ptr(glm::vec3(position)));
  glUniform4fv(location.gyration, 1, glm::value_ptr(glm::quat(gyration)));
  glUniform3fv(location.scale, 1, glm::value_ptr(scaling));
}

void H3Transformation::invert()
{
  position = -position;
  // position = glm::vec3(1/position.x, 1/position.y, 1/position.z);
  gyration = glm::inverse(gyration);
  // gyration = glm::quat(gyration.w, -gyration.x, -gyration.y, -gyration.z);
}

glm::vec3 H3Transformation::getPosition()
{
  return position;
}

void H3Transformation::scale(double scale)
{
  this->scaling.x *= scale;
  this->scaling.y *= scale;
  this->scaling.z *= scale;
}
void H3Transformation::scale(glm::vec3 scale)
{
  this->scaling *= scale;
}

// H3Transformation &H3Transformation::operator*=(const glm::quat &rhs)
// {
//   return *this;
// }
// H3Transformation &H3Transformation::operator+=(const glm::vec3 &rhs)
// {
//   return *this;
// }

//////////////////////////

H2ETransformation::H2ETransformation()
    : Gyrovector(), y(0), scaling(1) {}

H2ETransformation::Location H2ETransformation::uniformLocation(GLuint program, const char *name)
{
  H2ETransformation::Location loc;
  loc.position = glGetUniformLocation(program, (std::string(name) + ".position").c_str());
  loc.rotation = glGetUniformLocation(program, (std::string(name) + ".rotation").c_str());
  loc.scale = glGetUniformLocation(program, (std::string(name) + ".scale").c_str());
  return loc;
}
void H2ETransformation::bind(Location location)
{
  glm::vec3 p = glm::vec3(position.x, y, position.z);
  glUniform3fv(location.position, 1, glm::value_ptr(p));
  glUniform4fv(location.rotation, 1, glm::value_ptr(glm::quat(gyration)));
  glUniform3fv(location.scale, 1, glm::value_ptr(scaling));
}

H2ETransformation &H2ETransformation::operator*=(const glm::quat &rhs)
{
  Gyrovector::operator*=(rhs);
  return *this;
}

extern glm::dquat getGyration(const glm::dvec3 &u, const glm::dvec3 &v);

H2ETransformation &H2ETransformation::operator+=(const glm::vec3 &rhs)
{
  y += rhs.y;

  if (rhs.x != 0 || rhs.z != 0)
  {
    glm::dvec3 v = rhs;
    v = v * glm::dquat(gyration.w, -gyration.x, -gyration.y, -gyration.z);
    // y += v.y;
    v.y = 0;

    glm::dvec3 u = mobiusAddition(position, v);
    gyration = gyration * getGyration(u, position + v);
    position = u;
  }
  return *this;
}

// H2ETransformation &H2ETransformation::operator*(const glm::quat &rhs)
// {
//   H2ETransformation t;
//   t.gyration = gyration;
//   t.rotation = rotation;
//   t.position = position;
//   t.z = z;
//   t *= rhs;
//   return t;
// }

// H2ETransformation &H2ETransformation::operator+(const glm::vec3 &rhs)
// {
//   H2ETransformation t;
//   t.gyration = gyration;
//   t.rotation = rotation;
//   t.position = position;
//   t.z = z;
//   t += rhs;
//   return t;
// }

void H2ETransformation::invert()
{
  position = -position;
  y = -y;
  gyration = glm::inverse(gyration);
}

glm::vec3 H2ETransformation::getPosition()
{
  return glm::vec3(position.x, y, position.y);
}
void H2ETransformation::scale(double scale)
{
  this->scaling.x *= scale;
  this->scaling.y *= scale;
  this->scaling.z *= scale;
}
void H2ETransformation::scale(glm::vec3 scale)
{
  this->scaling *= scale;
}

//////////////////////////

H3LockedTransformation::H3LockedTransformation() : H3Transformation(), height(0) {}

H3Transformation::Location H3LockedTransformation::uniformLocation(GLuint program, const char *name)
{
  return H3Transformation::uniformLocation(program, name);
}

extern glm::dquat getGyration(const glm::dvec3 &u, const glm::dvec3 &v);
void H3LockedTransformation::bind(H3Transformation::Location location)
{
  glm::dvec3 pos = position;
  glm::dquat gyr = gyration;
  if (height != 0)
  {
    position = Gyrovector::mobiusAddition(position, glm::dvec3(0, height, 0));
    gyration *= glm::inverse(getGyration(position, pos + glm::dvec3(0, height, 0)));
  }
  H3Transformation::bind(location);
  position = pos;
  gyration = gyr;
}

H3LockedTransformation &H3LockedTransformation::operator*=(const glm::quat &rhs)
{
  H3Transformation::operator*=(rhs);
  return *this;
}
H3LockedTransformation &H3LockedTransformation::operator+=(const glm::vec3 &rhs)
{
  height = Gyrovector::mobiusAddition(glm::dvec3(0, height, 0), glm::dvec3(0, rhs.y, 0)).y;
  if (rhs.x != 0 || rhs.z != 0)
    H3Transformation::operator+=(glm::vec3(rhs.x, 0, rhs.z));
  return *this;
}

void H3LockedTransformation::invert()
{
  height = -height;
  H3Transformation::invert();
}
glm::vec3 H3LockedTransformation::getPosition()
{
  return Gyrovector::mobiusAddition(position, glm::vec3(0, height, 0));
}
