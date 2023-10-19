#include "Model.hpp"
#include <stdexcept>
#include <iostream>

ModelBuilder::ModelBuilder(std::string name)
{
  model = std::shared_ptr<Model>(new Model(name));
  state = 0;
}

void ModelBuilder::addVertices(std::vector<Vertex> &vertices)
{
  if (state != 0)
    throw std::runtime_error("Model builder bad state. Can not add vertices at current state.");

  verticesCombined.insert(verticesCombined.end(), vertices.begin(), vertices.end());
}

void ModelBuilder::addVertex(Vertex &vertex)
{
  if (state != 0)
    throw std::runtime_error("Model builder bad state. Can not add vertices at current state.");
  verticesCombined.push_back(vertex);
}

void ModelBuilder::addMaterials(std::vector<std::shared_ptr<Material>> &materials)
{
  if (state != 0)
    throw std::runtime_error("Model builder bad state. Can not add materials at current state.");

  mats.insert(mats.end(), materials.begin(), materials.end());
}

void ModelBuilder::addMaterial(std::shared_ptr<Material> material)
{
  if (state != 0)
    throw std::runtime_error("Model builder bad state. Can not add materials at current state.");

  mats.push_back(material);
}

void ModelBuilder::addMesh(std::string name, std::vector<GLuint> &indices, std::string materialName)
{
  if (state != 0)
    throw std::runtime_error("Model builder bad state. Can not add meshes at current state.");

  for (std::shared_ptr<Material> mat : mats)
  {
    if (mat->name == materialName)
    {
      std::shared_ptr<Mesh> mesh(new Mesh(name, mat));
      mesh->count = indices.size();
      mesh->offset = indicesCombined.size();

      meshes.push_back(mesh);
      indicesCombined.insert(indicesCombined.end(), indices.begin(), indices.end());
      return;
    }
  }
  throw std::runtime_error("Model builder can not resolve material.");
}

std::shared_ptr<Model> ModelBuilder::build()
{
  state = 1;
  glGenVertexArrays(1, &model->vertexArray);
  glGenBuffers(1, &model->vertexBuffer);
  glGenBuffers(1, &model->indexBuffer);

  glBindVertexArray(model->vertexArray);
  glBindBuffer(GL_ARRAY_BUFFER, model->vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, verticesCombined.size() * sizeof(Vertex), &verticesCombined[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCombined.size() * sizeof(GLuint), indicesCombined.data(), GL_STATIC_DRAW);

  // glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)(0));
  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)(sizeof(glm::vec4)));
  // glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)(sizeof(glm::vec4) + sizeof(glm::vec3)));
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void *)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void *)(4 * sizeof(GLfloat)));
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void *)(7 * sizeof(GLfloat)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  model->meshes = meshes;
  model->materials = mats;

  return model;
}

Model::Model(std::string name) : name(name) {}

Model::~Model()
{
  glDeleteBuffers(1, &vertexBuffer);
  glDeleteBuffers(1, &indexBuffer);
  glDeleteVertexArrays(1, &vertexArray);
}

void Model::draw(Material::Location materialLocation)
{
  glBindVertexArray(vertexArray);
  for (std::shared_ptr<Mesh> mesh : meshes)
  {
    mesh->draw(materialLocation);
  }
}