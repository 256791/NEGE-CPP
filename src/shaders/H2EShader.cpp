#include "H2EShader.hpp"

#include <iostream>
#include "../components/Light.hpp"

#ifndef RELEASE
const char *H2EShader::vertPaths[1] = {
    "../res/glsl/H2E.vert"};
const int H2EShader::vertCount = 1;

const char *H2EShader::fragPaths[1] = {
    "../res/glsl/E3.frag"};
const int H2EShader::fragCount = 1;

H2EShader::H2EShader()
{
  initFromFiles(vertPaths, vertCount, std::vector<std::string>(), fragPaths, fragCount, std::vector<std::string>());

  uModel = H2ETransformation::uniformLocation(program, "uModel");
  uView = H2ETransformation::uniformLocation(program, "uView");
  uCameraPosition = glGetUniformLocation(program, "uCameraPosition");
  uProjection = glGetUniformLocation(program, "uProjection");

  uCurrentMaterial = Material::uniformLocation(program, "uCurrentMaterial");

  uLight = LightCollection::uniformLocation(program, "light", 8, 4, 4);
}
#endif

void H2EShader::clear()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}