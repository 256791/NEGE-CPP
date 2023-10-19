#ifndef MESH_HPP
#define MESH_HPP
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>

#include "Material.hpp"


class Mesh
{
public:
  void draw(Material::Location materialLocation);


private:
  friend class ModelBuilder;

  Mesh(std::string name, std::shared_ptr<Material> material);
  std::string name;
  
  int count;
  int offset;

  std::shared_ptr<Material> material;
};
#endif