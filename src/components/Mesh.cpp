#include "Mesh.hpp"

Mesh::Mesh(std::string name, std::shared_ptr<Material> material)
    : name(name), material(material) {}

void Mesh::draw(Material::Location materialLocation) {
  material->bind(materialLocation);
  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
}
