#include "Material.hpp"
#include <glm/ext.hpp>

Material::Material(std::string name)
    : name(name), Ka(1), Kd(1), Ks(0), Ni(1), Ns(0), d(1), illum(1) {}

Material::Location Material::uniformLocation(GLuint program, const char *name)
{
  Material::Location loc;
  loc.Ka = glGetUniformLocation(program, (std::string(name) + ".Ka").c_str());
  loc.Kd = glGetUniformLocation(program, (std::string(name) + ".Kd").c_str());
  loc.Ks = glGetUniformLocation(program, (std::string(name) + ".Ks").c_str());
  loc.Ni = glGetUniformLocation(program, (std::string(name) + ".Ni").c_str());
  loc.Ns = glGetUniformLocation(program, (std::string(name) + ".Ns").c_str());
  loc.d = glGetUniformLocation(program, (std::string(name) + ".d").c_str());
  loc.illum = glGetUniformLocation(program, (std::string(name) + ".illum").c_str());

  return loc;
}

void Material::bind(Location location)
{
  glUniform3fv(location.Ka, 1, glm::value_ptr(Ka));
  glUniform3fv(location.Kd, 1, glm::value_ptr(Kd));
  glUniform3fv(location.Ks, 1, glm::value_ptr(Ks));
  glUniform1f(location.Ni, Ni);
  glUniform1f(location.Ns, Ns);
  glUniform1f(location.d, d);
  glUniform1i(location.illum, illum);
}
