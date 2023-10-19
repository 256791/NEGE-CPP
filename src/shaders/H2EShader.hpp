#ifndef H2E_SHADER
#define H2E_SHADER

#include "Shader.hpp"
#include "../components/Transformation.hpp"
#include "../components/Material.hpp"
#include "../components/Light.hpp"

class H2EShader : public Shader
{
public:
  H2EShader();

  void clear();

  H2ETransformation::Location uModel;
  H2ETransformation::Location uView;
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