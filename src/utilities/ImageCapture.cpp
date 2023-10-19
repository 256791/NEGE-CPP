#include "ImageCapture.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

void capture(GLFWwindow *window, const char *filename)
{
  int w, h;
  glfwGetFramebufferSize(window, &w, &h);
  int stride = 3 * w;
  stride += (stride % 4) ? (4 - stride % 4) : 0;
  int size = stride * h;
  char *buffer = new char[size];

  glPixelStorei(GL_PACK_ALIGNMENT, 4);
  glReadBuffer(GL_FRONT);
  glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, buffer);

  stbi_flip_vertically_on_write(true);
  stbi_write_png(filename, w, h, 3, buffer, stride);

  delete[] buffer;
}

ImageCaptutre::ImageCaptutre(std::string filename, Mode mode, int interval)
    : filename(filename), mode(mode), image(0), counter(0), interval(interval)
{
}

bool ImageCaptutre::handle(Engine *engine)
{
  if (mode == Mode::SINGLE_IMAGE)
  {
    capture(engine->getWindow(), (filename + ".png").c_str());
    return true;
  }
  if (mode == Mode::INTERVAL)
  {
    if (counter % interval == 0)
    {
      capture(engine->getWindow(), (filename + "_" + std::to_string(image) + ".png").c_str());
      image++;
    }
    counter++;
    return false;
  }
  if (mode == Mode::CONTINOUS)
  {
    capture(engine->getWindow(), (filename + "_" + std::to_string(image) + ".png").c_str());
    image++;
    return false;
  }
  return true;
}

ImageCaptutre::~ImageCaptutre()
{
  
}