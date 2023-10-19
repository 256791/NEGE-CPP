#include "Obj.hpp"
#include <glad/glad.h>
#include <map>

struct indicesCompare
{
  bool const operator()(const glm::ivec3 &lhs, const glm::ivec3 rhs) const
  {
    if (lhs.x != rhs.x)
      return lhs.x < rhs.x;
    if (lhs.y != rhs.y)
      return lhs.y < rhs.y;
    return lhs.z < rhs.z;
  }
};

Obj::Face::Face(glm::ivec3 v, glm::ivec3 u, glm::ivec3 n)
    : vertInd(v), uvInd(u), normInd(n)
{}

Obj::Obj()
{
  name = "";
  groups.push_back(Group());
}

// TODO add groups
std::shared_ptr<Model> Obj::toModel(std::vector<std::shared_ptr<Material>> &materials)
{
  ModelBuilder builder(name);
  std::map<glm::ivec3, int, indicesCompare> order;
  int index = 0;


  builder.addMaterial(std::shared_ptr<Material>(new Material("")));
  builder.addMaterials(materials);

  for (Group group : groups)
  {
    if(group.faces.size() == 0)
      continue;
    std::vector<GLuint> indices;
    for (Face face : group.faces)
    {
      for (int i = 0; i < 3; i++)
      {
        // FIXME negative indexing and 0 index(no data)
        glm::ivec3 transposed(face.vertInd[i], face.normInd[i], face.uvInd[i]);
        std::map<glm::ivec3, int>::iterator it = order.find(transposed);
        if (it == order.end())
        {
          indices.push_back(index);
          order.insert(std::pair<glm::ivec3, int>(transposed, index++));
          Vertex vertex;
          if (transposed[0] == 0)
            vertex.position = glm::vec4(0);
          else
            vertex.position = vertices[transposed[0] - 1];
          if (transposed[1] == 0)
            vertex.normal = glm::vec3(0);
          else
            vertex.normal = normals[transposed[1] - 1];
          if (transposed[2] == 0)
            vertex.uv = glm::vec3(0);
          else
            vertex.uv = uvs[transposed[2] - 1];

          builder.addVertex(vertex);
        }
        else
          indices.push_back(it->second);
      }
    }
    builder.addMesh(group.name, indices, group.materialName);
  }

  return builder.build();
}