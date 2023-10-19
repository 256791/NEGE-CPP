#ifndef TRANSFORMATION_HPP
#define TRANSFORMATION_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Gyrovector.hpp"

#define MAKE_TRANSFORMATION_TEMPLATE_TYPES(Base) \
  template class Base<H3Transformation>;         \
  template class Base<E3Transformation>;         \
  template class Base<H3LockedTransformation>;   \
  template class Base<H2ETransformation>;

class Transformation
{
public:
  Transformation();
  // virtual void translate(glm::vec3 translation) = 0;
  // virtual void rotate(glm::vec3 angles) = 0;
  // void rotate(glm::quat quaterion) = 0;
  // virtual ~Transformation() = default;
};

class E3Transformation : public Transformation, public glm::mat4
{
public:
  E3Transformation();

  typedef GLuint Location;
  // struct Location{
  //   GLuint matrix;
  // };

  static E3Transformation::Location uniformLocation(const GLuint program, const char *name);
  void bind(Location location);


  E3Transformation &operator*=(const glm::quat &rhs);
  E3Transformation &operator*=(const glm::mat4 &rhs);
  E3Transformation &operator+=(const glm::vec3 &rhs);

  // E3Transformation &operator*(const glm::mat4 &rhs);
  void operator=(const glm::mat4 &rhs);
  void invert();
  glm::vec3 getPosition();
  void scale(double scale);
  void scale(glm::vec3 scale);

  glm::vec3 scaling;
};

class H3Transformation : public Transformation, public Gyrovector
{
public:
  H3Transformation();

  struct Location
  {
    GLuint position;
    GLuint gyration;
    GLuint scale;
  };

  static Location uniformLocation(GLuint program, const char *name);
  void bind(Location location);

  // H3Transformation &operator*=(const glm::quat &rhs);
  // H3Transformation &operator+=(const glm::vec3 &rhs);
  void invert();
  glm::vec3 getPosition();
  void scale(double scale);
  void scale(glm::vec3 scale);



  glm::vec3 scaling;
};

class H3LockedTransformation : public H3Transformation
{
public:
  H3LockedTransformation();

  static Location uniformLocation(GLuint program, const char *name);
  void bind(Location location);

  H3LockedTransformation &operator*=(const glm::quat &rhs);
  H3LockedTransformation &operator+=(const glm::vec3 &rhs);
  void invert();
  glm::vec3 getPosition();

  glm::vec3 scaling;
  double height;
};

class H2ETransformation : public Transformation, private Gyrovector
{
public:
  H2ETransformation();

  struct Location
  {
    GLuint position;
    GLuint rotation;
    GLuint scale;
  };

  static Location uniformLocation(GLuint program, const char *name);
  void bind(Location location);

  H2ETransformation &operator*=(const glm::quat &rhs);
  H2ETransformation &operator+=(const glm::vec3 &rhs);
  // H2ETransformation &operator*(const glm::quat &rhs);
  // H2ETransformation &operator+(const glm::vec3 &rhs);
  void invert();
  glm::vec3 getPosition();
  void scale(double scale);
  void scale(glm::vec3 scale);
  

  double y;
  glm::vec3 scaling;
};

#endif