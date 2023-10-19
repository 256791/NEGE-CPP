#ifndef H3_SHADER
#define H3_SHADER

#include "Shader.hpp"
#include "../components/Transformation.hpp"
#include "../components/Material.hpp"
#include "../components/Light.hpp"

class H3Shader : public Shader
{
public:
  H3Shader();

  void clear();

  H3Transformation::Location uModel;
  H3Transformation::Location uView;
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