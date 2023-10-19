#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

class Light
{
public:
protected:
  glm::vec3 color;
  bool castShadows;
};

class PointLight : public Light
{
public:
  PointLight(glm::vec3 position, glm::vec3 color = glm::vec3(1.f),
             float constant = 1.f, float linear = 0.2f, float quadratic = 0.05f,
             bool castShadows = false);

  struct Location{
    GLuint color;
    GLuint position;
    GLuint constant;
    GLuint linear;
    GLuint quadratic;
  };
  static Location uniformLocation(const GLuint program, const char *name);
  void bind(Location location);

protected:
  glm::vec3 position;
  float constant;
  float linear;
  float quadratic;
};

class DirectionalLight : public Light
{
public:
  DirectionalLight(glm::vec3 direction, glm::vec3 color = glm::vec3(1.f), bool castShadows = false);

  struct Location{
    GLuint color;
    GLuint direction;
  };
  static Location uniformLocation(const GLuint program, const char *name);
  void bind(Location location);
  
protected:
  glm::vec3 direction;
};

class Spotlight : public Light
{
public:
  Spotlight(glm::vec3 position, glm::vec3 direction, glm::vec3 color = glm::vec3(1.f),
            float focus = 60.f, float angle = 90.f, bool castShadows = false);

  struct Location{
    GLuint color;
    GLuint position;
    GLuint direction;
    GLuint focus;
    GLuint angle;
  };
  static Location uniformLocation(const GLuint program, const char *name);
  void bind(Location location);


protected:
  glm::vec3 position;
  glm::vec3 direction;
  float focus;
  float angle;
};

class LightCollection{
public:
  struct Location{
    std::vector<Spotlight::Location> spot;
    std::vector<DirectionalLight::Location> directional;
    std::vector<PointLight::Location> point;
    GLuint spot_len;
    GLuint directional_len;
    GLuint point_len;
  };
  static Location uniformLocation(const GLuint program, const char *namePrefix, int maxPoint=1, int maxDir=1, int maxSpot=0);
  void bind(Location location);

  void addLight(Spotlight light);
  void addLight(DirectionalLight light);
  void addLight(PointLight light);
  //void sort(glm::vec3 position);

private:
  std::vector<Spotlight> spot;
  std::vector<DirectionalLight> directional;
  std::vector<PointLight> point;
};


#endif