#ifndef OBJ_HPP
#define OBJ_HPP

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include "../components/Model.hpp"

struct Obj{
  Obj();
  std::shared_ptr<Model> toModel(std::vector<std::shared_ptr<Material>> &materials);

  struct Face{
    Face();
    Face(glm::ivec3 v = glm::ivec3(0), glm::ivec3 u = glm::ivec3(0), glm::ivec3 n = glm::ivec3(0));
    glm::ivec3 vertInd;
    glm::ivec3 uvInd;
    glm::ivec3 normInd;
  };
  struct Group{
    std::string name;
    std::string materialName = "";
    std::vector<Face> faces;
  };

  std::string name;
  std::vector<Group> groups;
  std::vector<glm::vec4> vertices;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec3> uvs;
};

#endif