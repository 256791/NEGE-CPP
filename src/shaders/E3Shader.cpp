#include "E3Shader.hpp"

#include <iostream>

#ifndef RELEASE
const char *E3Shader::vertPaths[1] = {
    "../res/glsl/E3.vert"};
const int E3Shader::vertCount = 1;

const char *E3Shader::fragPaths[1] = {
    "../res/glsl/E3.frag"};
const int E3Shader::fragCount = 1;

E3Shader::E3Shader()
{
  initFromFiles(vertPaths, vertCount, std::vector<std::string>(), fragPaths, fragCount, std::vector<std::string>());

  uModel = E3Transformation::uniformLocation(program, "uModel");
  uView = E3Transformation::uniformLocation(program, "uView");
  uCameraPosition = E3Transformation::uniformLocation(program, "uCameraPosition");
  uProjection = glGetUniformLocation(program, "uProjection");

  uCurrentMaterial = Material::uniformLocation(program, "uCurrentMaterial");

  uLight = LightCollection::uniformLocation(program, "light", 8, 4, 4);
}
#endif

void E3Shader::clear()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}