#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>

#include "Texture.hpp"

class Material
{
public:
  Material(std::string name);

  struct Location
  {
    GLuint Ka;
    GLuint Kd;
    GLuint Ks;
    GLuint Ni;
    GLuint Ns;
    GLuint d;
    GLuint illum;
  };

  static Location uniformLocation(GLuint program, const char *name);

  void bind(Location location);

// private:
  // friend class ModelBuilder;

  std::string name;

  glm::vec3 Ka;
  glm::vec3 Kd;
  glm::vec3 Ks;

  GLfloat Ni;
  GLfloat Ns;
  GLfloat d;
  GLuint illum;

  std::shared_ptr<Texture> map_Ka;
  std::shared_ptr<Texture> map_Kd;
  std::shared_ptr<Texture> map_Ks;
};

#endif