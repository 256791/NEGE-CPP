#include "Shader.hpp"
#include <fstream>
#include <iostream>

extern std::string relpath(std::string path);

Shader::Shader()
{
  initialized = false;
}

Shader::~Shader()
{
  if (initialized)
    glDeleteProgram(program);
}

void Shader::use()
{
  if (!initialized)
    throw std::runtime_error("Shader is not initialized");
  glUseProgram(program);
}

#ifndef RELEASE
void Shader::initFromFiles(const char **vertPaths, const int vertCount, std::vector<std::string> vertDefine,
                           const char **fragPaths, const int fragCount, std::vector<std::string> fragDefine)
{
  std::vector<const char *> fragSources, vertSources = Shader::loadShaderFromFiles(vertPaths, vertCount);
  try
  {
    fragSources = Shader::loadShaderFromFiles(fragPaths, fragCount);
  }
  catch (...)
  {
    for (const char *s : vertSources)
      delete[] s;
    std::rethrow_exception(std::current_exception());
  }

  try
  {
    init(&vertSources[0], vertSources.size(), vertDefine, &fragSources[0], fragSources.size(), fragDefine);
  }
  catch (...)
  {
    for (const char *s : vertSources)
      delete[] s;
    for (const char *s : fragSources)
      delete[] s;
    std::rethrow_exception(std::current_exception());
  }

  for (const char *s : vertSources)
    delete[] s;
  for (const char *s : fragSources)
    delete[] s;
}
#endif

void Shader::init(const char **vertSources, const int vertCount, std::vector<std::string> vertDefine,
                  const char **fragSources, const int fragCount, std::vector<std::string> fragDefine)
{
  GLuint vertShader, fragShader, linked;

  vertShader = compileShader(GL_VERTEX_SHADER, vertSources, vertCount, vertDefine);

  try
  {
    fragShader = compileShader(GL_FRAGMENT_SHADER, fragSources, fragCount, fragDefine);
  }
  catch (...)
  {
    glDeleteShader(vertShader);
    std::rethrow_exception(std::current_exception());
  }

  try
  {
    linked = linkProgram(vertShader, fragShader);
  }
  catch (...)
  {
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    std::rethrow_exception(std::current_exception());
  }

  glDeleteShader(vertShader);
  glDeleteShader(fragShader);
  if (initialized)
    glDeleteProgram(program);
  program = linked;
  initialized = true;
}

GLuint Shader::linkProgram(GLuint vertexShader, GLuint fragmentShader)
{
  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  int status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (!status)
  {
    char log[1024] = "Program compile error: \n";
    glGetProgramInfoLog(program, 1000, NULL, log + sizeof(char) * 24);
    glDeleteProgram(program);
    throw std::runtime_error(log);
  }

  return program;
}

#ifndef RELEASE
std::vector<const char *> Shader::loadShaderFromFiles(const char **paths, const int count)
{
  std::vector<const char *> sources;
  for (int i = 0; i < count; i++)
  {
    std::ifstream file(relpath(paths[i]), std::ios::binary);
    if (!file.is_open())
    {
      for (int j = 0; j < i; j++)
        delete[] sources[j];
      throw std::runtime_error("Can not open shader file");
    }

    int len, offset = 0;
    std::string first;
    file >> std::skipws >> first;
    if (first == "#version")
    {
      getline(file, first);
      offset = file.tellg();
    }
    file.seekg(0, std::ios::end);
    len = file.tellg();
    file.seekg(offset, std::ios::beg);

    char *data;
    data = new char[len + 1 - offset];
    file.read(data, len);
    file.close();
    data[len - offset] = 0;
    sources.push_back(data);
  }
  return sources;
}
#endif

GLuint Shader::compileShader(GLenum shaderType, const char **sources, const int count, std::vector<std::string> define)
{
  const char **combined = new const char *[count + 1];

  std::string def = GLSL_VERSION;
  for (std::string define : define)
    def += "#define " + define + "\n";
  combined[0] = def.c_str();

  for (int i = 0; i < count; i++)
    combined[i + 1] = sources[i];

  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, count + 1, combined, NULL);
  glCompileShader(shader);

  delete[] combined;

  int status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (!status)
  {
    char log[1024] = "Shader compile error:  \n";
    glGetShaderInfoLog(shader, 1000, NULL, log + sizeof(char) * 24);
    glDeleteShader(shader);
    throw std::runtime_error(log);
  }

  return shader;
}