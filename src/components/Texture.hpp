#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>
#include <string>
#include <stdexcept>

extern std::string relpath(std::string path);

class Texture
{
public:
  Texture(std::string path);
};

#endif