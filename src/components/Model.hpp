#ifndef MODEL_HPP
#define MODEL_HPP
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>

#include "Material.hpp"
#include "Mesh.hpp"

struct Vertex
{
  glm::vec4 position;
  glm::vec3 normal;
  glm::vec3 uv;
};

class ModelBuilder;
class Model
{
public:
  ~Model();
  void draw(Material::Location materialLocation);
  const std::string getName() const { return name; }

private:
  friend class ModelBuilder;

  Model(std::string name);

  std::string name;
  GLuint indexBuffer;
  GLuint vertexArray;
  GLuint vertexBuffer;

  std::vector<std::shared_ptr<Material>> materials;
  std::vector<std::shared_ptr<Mesh>> meshes;
};

class ModelBuilder
{
public:
  ModelBuilder(std::string name = "undefined");
  void addVertices(std::vector<Vertex> &vertices);
  void addVertex(Vertex &vertex);
  void addMaterials(std::vector<std::shared_ptr<Material>> &materials);
  void addMaterial(std::shared_ptr<Material> material);
  void addMesh(std::string name, std::vector<GLuint> &indices, std::string materialName);
  std::shared_ptr<Model> build();

private:
  std::shared_ptr<Model> model;
  std::vector<std::shared_ptr<Material>> mats;
  std::vector<std::shared_ptr<Mesh>> meshes;

  std::vector<Vertex> verticesCombined;
  std::vector<GLuint> indicesCombined;
  int state;
};

#endif