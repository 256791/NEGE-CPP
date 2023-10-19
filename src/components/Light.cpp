#include "Light.hpp"
#include <glm/ext.hpp>
#include <string>

PointLight::PointLight(glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic, bool castShadows)
    : position(position), constant(constant), linear(linear), quadratic(quadratic)
{
  this->color = color;
  this->castShadows = castShadows;
}
PointLight::Location PointLight::uniformLocation(const GLuint program, const char *name)
{
  PointLight::Location loc;
  loc.position = glGetUniformLocation(program, (std::string(name) + ".position").c_str());
  loc.color = glGetUniformLocation(program, (std::string(name) + ".color").c_str());
  loc.constant = glGetUniformLocation(program, (std::string(name) + ".constant").c_str());
  loc.linear = glGetUniformLocation(program, (std::string(name) + ".linear").c_str());
  loc.quadratic = glGetUniformLocation(program, (std::string(name) + ".quadratic").c_str());
  return loc;
}
void PointLight::bind(Location location)
{
  glUniform3fv(location.position, 1, glm::value_ptr(position));
  glUniform3fv(location.color, 1, glm::value_ptr(color));
  glUniform1f(location.constant, constant);
  glUniform1f(location.linear, linear);
  glUniform1f(location.quadratic, quadratic);
}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color, bool castShadows)
    : direction(glm::normalize(direction))
{
  this->color = color;
  this->castShadows = castShadows;
}
DirectionalLight::Location DirectionalLight::uniformLocation(const GLuint program, const char *name)
{
  DirectionalLight::Location loc;
  loc.direction = glGetUniformLocation(program, (std::string(name) + ".direction").c_str());
  loc.color = glGetUniformLocation(program, (std::string(name) + ".color").c_str());
  return loc;
}
void DirectionalLight::bind(Location location)
{
  glUniform3fv(location.color, 1, glm::value_ptr(color));
  glUniform3fv(location.direction, 1, glm::value_ptr(direction));
}

Spotlight::Spotlight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float focus, float angle, bool castShadows)
    : position(position), direction(direction), focus(focus), angle(angle)
{
  this->color = color;
  this->castShadows = castShadows;
}
Spotlight::Location Spotlight::uniformLocation(const GLuint program, const char *name)
{
  Spotlight::Location loc;
  loc.position = glGetUniformLocation(program, (std::string(name) + ".position").c_str());
  loc.direction = glGetUniformLocation(program, (std::string(name) + ".direction").c_str());
  loc.color = glGetUniformLocation(program, (std::string(name) + ".color").c_str());
  loc.focus = glGetUniformLocation(program, (std::string(name) + ".focus").c_str());
  loc.angle = glGetUniformLocation(program, (std::string(name) + ".angle").c_str());
  return loc;
}
void Spotlight::bind(Location location)
{
  glUniform3fv(location.position, 1, glm::value_ptr(position));
  glUniform3fv(location.direction, 1, glm::value_ptr(direction));
  glUniform3fv(location.color, 1, glm::value_ptr(color));
  glUniform1f(location.focus, focus);
  glUniform1f(location.angle, angle);
}

LightCollection::Location LightCollection::uniformLocation(const GLuint program, const char *namePrefix,
                                                           int maxPoint, int maxDir, int maxSpot)
{
  LightCollection::Location loc;
  for (int i = 0; i < maxPoint; i++)
  {
    loc.point.push_back(PointLight::uniformLocation(program, (std::string(namePrefix) + "_point[" + std::to_string(i) + "]").c_str()));
  }
  for (int i = 0; i < maxDir; i++)
  {
    loc.directional.push_back(DirectionalLight::uniformLocation(program, (std::string(namePrefix) + "_directional[" + std::to_string(i) + "]").c_str()));
  }
  for (int i = 0; i < maxSpot; i++)
  {
    loc.spot.push_back(Spotlight::uniformLocation(program, (std::string(namePrefix) + "_spot[" + std::to_string(i) + "]").c_str()));
  }

  loc.spot_len = glGetUniformLocation(program, (std::string(namePrefix) + "_spot_arrsize").c_str());
  loc.directional_len = glGetUniformLocation(program, (std::string(namePrefix) + "_directional_arrsize").c_str());
  loc.point_len = glGetUniformLocation(program, (std::string(namePrefix) + "_point_arrsize").c_str());

  return loc;
}

void LightCollection::bind(Location location)
{
  for (long long unsigned int i = 0; i < point.size() && i < location.point.size(); i++)
  {
    point[i].bind(location.point[i]);
  }
  for (long long unsigned int i = 0; i < directional.size() && i < location.directional.size(); i++)
  {
    directional[i].bind(location.directional[i]);
  }
  for (long long unsigned int i = 0; i < spot.size() && i < location.spot.size(); i++)
  {
    spot[i].bind(location.spot[i]);
  }
  
  glUniform1i(location.spot_len, int(std::min(spot.size(), location.spot.size())));
  glUniform1i(location.directional_len, int(std::min(directional.size(), location.directional.size())));
  glUniform1i(location.point_len, int(std::min(point.size(), location.point.size())));
}

void LightCollection::addLight(Spotlight light)
{
  spot.push_back(light);
}

void LightCollection::addLight(DirectionalLight light)
{
  directional.push_back(light);
}

void LightCollection::addLight(PointLight light)
{
  point.push_back(light);
}