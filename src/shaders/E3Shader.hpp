#ifndef E3_SHADER
#define E3_SHADER

#include "Shader.hpp"
#include "../components/Transformation.hpp"
#include "../components/Material.hpp"
#include "../components/Light.hpp"

class E3Shader : public Shader
{
public:
  E3Shader();

  void clear();

  E3Transformation::Location uModel;
  E3Transformation::Location uView;
  GLuint uCameraPosition;
  GLuint uProjection;

  Material::Location uCurrentMaterial;
  LightCollection::Location uLight;

private:
#ifndef RELEASE
  static const char *vertPaths[1];
  static const int vertCount;
  static const char *fragPaths[1];
  static const int fragCount;
#endif
};
#endif