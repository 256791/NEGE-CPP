#ifndef SHADER_HPP
#define SHADER_HPP
#include <glad/glad.h>
#include <string>
#include <vector>

#define GLSL_VERSION "#version 330 core\n"

/**
 * @brief GLSL Shader program base class
 */
class Shader
{
public:
  Shader();
  ~Shader();

  void use();

protected:
  GLuint program;
  bool initialized;

#ifndef RELEASE
  /**
   * @brief Initializes shader from files. Omitted in release mode.
   *
   * @param vertPaths Vertex shader files to be compiled.
   * @param vertCount Number of vertex shader files.
   * @param vertDefine Vector of strings contataining configuration flags for vertex shader (conveted to #define $(string)).
   * @param fragPaths Fragment shader files to be compiled.
   * @param fragCount Number of fragment shader files.
   * @param fragDefine Vector of strings contataining configuration flags for fragment shader (conveted to #define $(string)).
   */
  void initFromFiles(const char **vertPaths, const int vertCount, std::vector<std::string> vertDefine,
                     const char **fragPaths, const int fragCount, std::vector<std::string> fragDefine);
#endif

  /**
   * @brief Initializes shader from sources.
   *
   * @param vertSources Vertex shader sources to be compiled.
   * @param vertCount Number of vertex shader sources.
   * @param vertDefine Vector of strings contataining configuration flags for vertex shader (conveted to #define $(string)).
   * @param fragSources Fragment shader sources to be compiled.
   * @param fragCount Number of fragment shader sources.
   * @param fragDefine Vector of strings contataining configuration flags for fragment shader (conveted to #define $(string)).
   */
  void init(const char **vertSources, const int vertCount, std::vector<std::string> vertDefine,
            const char **fragSources, const int fragCount, std::vector<std::string> fragDefine);

private:
  /**
   * @brief Links program.
   *
   * @param vertexShader Compiled vertex shader.
   * @param fragmentShader Compiled fragment shader.
   * @return GLuint Linked program.
   *
   * @throws std::runtime_error on program linking error.
   */
  static GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader);

#ifndef RELEASE
  /**
   * @brief Load shader from files. Omitted in release mode.
   *
   * @note Uses path relative to executable.
   * @note Removes #version tag from sources
   * @warning Returned vector contains const char arrays initialized with new keyword (needs cleanup)
   *
   * @param paths Paths to files to be loaded
   * @param count Number of files
   * @return std::vector<const char *> vector of sources.
   *
   * @throws std::runtime_error on file open error.
   */
  static std::vector<const char *> loadShaderFromFiles(const char **paths, const int count);
#endif

  /**
   * @brief Method used to compile shader from sources.
   *
   * @param shaderType Either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
   * @param sources Sources to be compiled.
   * @param count Number of sources.
   * @param define Vector of strings contataining configuration flags for program (conveted to #define $(string)).
   * @return GLuint compiled shader program.
   *
   * @throws std::runtime_error on shader compile error.
   */
  static GLuint compileShader(GLenum shaderType, const char **sources, const int count, std::vector<std::string> define);
};

#endif