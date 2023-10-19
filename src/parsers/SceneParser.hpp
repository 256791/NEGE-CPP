#ifndef SCENE_PARSER_HPP
#define SCENE_PARSER_HPP

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "../components/Scene.hpp"

class SceneLexer;

// Simple Parser instead bison
class SceneParser
{
public:
  SceneParser();
  ~SceneParser();
  void parse(std::string path);
  Scene * getScene();
  static Scene *loadScene(std::string path);

protected:
  friend class SceneLexer;
  enum Token
  {
    H3, E3, H3LOCK,
    TYPE, MODEL, ENTITY,
    T, S, R,
    VAL, IND, NAME,
    ENDL, ENDF, ERR
  };

  struct Symbol
  {
    Symbol(Token type);
    Symbol(float val);
    Symbol(int ind);
    Symbol(std::string name);
    Symbol(Token type, std::string name);

    Token type;
    float val;
    int ind;
    std::string name;
  };

private:
  SceneLexer *lexer;
  Scene *scene;
  glm::vec3 parseVector();
};
#endif