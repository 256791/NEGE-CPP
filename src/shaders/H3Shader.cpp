#include "H3Shader.hpp"

#include <iostream>

#ifndef RELEASE
const char *H3Shader::vertPaths[1] = {
    "../res/glsl/H3.vert"};
const int H3Shader::vertCount = 1;

const char *H3Shader::fragPaths[1] = {
    "../res/glsl/H3.frag"};
const int H3Shader::fragCount = 1;

H3Shader::H3Shader()
{
  initFromFiles(vertPaths, vertCount, std::vector<std::string>(), fragPaths, fragCount, std::vector<std::string>());

  uModel = H3Transformation::uniformLocation(program, "uModel");
  uView = H3Transformation::uniformLocation(program, "uView");
  uCameraPosition = glGetUniformLocation(program, "uCameraPosition");
  uProjection = glGetUniformLocation(program, "uProjection");

  uCurrentMaterial = Material::uniformLocation(program, "uCurrentMaterial");
  uLight = LightCollection::uniformLocation(program, "light", 8, 4, 4);
}
#endif

void H3Shader::clear()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}