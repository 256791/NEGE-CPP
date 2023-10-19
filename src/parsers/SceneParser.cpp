#include "SceneParser.hpp"
#include "SceneLexer.hpp"

#include "ObjParser.hpp"

#include <fstream>
#include <vector>
#include <filesystem>

#define GET_NAME(dest, lexer, error) \
  dest = lexer->get_next_token();    \
  if (dest.type != Token::NAME)      \
    throw std::runtime_error(error);

SceneParser::Symbol::Symbol(Token type) : type(type){};
SceneParser::Symbol::Symbol(float val) : type(Token::VAL), val(val){};
SceneParser::Symbol::Symbol(int ind) : type(Token::IND), ind(ind){};
SceneParser::Symbol::Symbol(std::string name) : type(Token::NAME), name(name){};
SceneParser::Symbol::Symbol(Token type, std::string name) : type(type), name(name){};

SceneParser::SceneParser() { lexer = new SceneLexer(); }
SceneParser::~SceneParser() { delete lexer; }
ObjParser oparser;

void SceneParser::parse(std::string path)
{
  std::ifstream file(path);
  if (!file.is_open())
    throw std::runtime_error("Can not open scene file");
  lexer->switch_streams(&file, NULL);

  std::string dir = path.substr(0, path.find_last_of('\\'));

  Symbol current = lexer->get_next_token();
  if (current.type == Token::H3)
    scene = new H3Scene();
  else if (current.type == Token::E3)
    scene = new E3Scene();
  else if (current.type == Token::H3LOCK)
    scene = new H3LockScene();
  else
    throw std::runtime_error("Wrong scene definition");

  while (true)
  {
    Symbol current = lexer->get_next_token();
    switch (current.type)
    {
    case Token::MODEL:
      current = lexer->get_next_token();
      if (current.type == Token::NAME)
      {
        oparser.parse(std::filesystem::path(path).replace_filename(current.name).string());
        for (auto m : oparser.getModels())
        {
          FOR_ANY_SCENE(scene, {
            any_scene->addModel(m);
          })
        }
        current = lexer->get_next_token();
        if (current.type != Token::ENDL && current.type != Token::ENDF)
          throw std::runtime_error("Wrong model definition");
      }
      else
        throw std::runtime_error("Wrong model definition");
      break;
    case Token::ENTITY:
      current = lexer->get_next_token();
      if (current.type == Token::NAME)
      {
        FOR_ANY_SCENE_TYPED(scene, {
          for (auto m : any_scene->models)
          {
            if (m->getName() == current.name)
            {
              any_scene->addEntity(Entity<any_scene_ttype>(m));
            }
          }
        })
        current = lexer->get_next_token();
        if (current.type != Token::ENDL && current.type != Token::ENDF)
          throw std::runtime_error("Wrong entity definition");
      }
      else
        throw std::runtime_error("Wrong entity definition");
      break;
    case Token::T:
      FOR_ANY_SCENE(scene, {
        *any_scene->entities.back().transformation += parseVector();
      })
      break;
    case Token::S:
      FOR_ANY_SCENE(scene, {
        any_scene->entities.back().transformation->scale(parseVector());
      })
      break;
    case Token::R:
    {
      glm::vec3 euler = parseVector();
      glm::quat r = glm::angleAxis(euler.x, glm::vec3(1, 0, 0)) * glm::angleAxis(euler.y, glm::vec3(0, 1, 0)) * glm::angleAxis(euler.z, glm::vec3(0, 0, 1));
      FOR_ANY_SCENE(scene, {
        *any_scene->entities.back().transformation *= r;
      })
    }
    break;
    case Token::ENDL:
      break;
    case Token::ENDF:
      return;
      break;
    default:
#ifdef DEBUG
      std::cerr << "Warning ignoring unsupported statement inside scene file " << current.type << "\n";
#endif
      break;
    }
  }
}

Scene *SceneParser::getScene()
{
  return scene;
}

glm::vec3 SceneParser::parseVector()
{
  float x, y, z;

  Symbol s = lexer->get_next_token();
  if (s.type == Token::VAL)
    x = s.val;
  else if (s.type == Token::IND)
    x = s.ind;
  else
    throw std::runtime_error("Wrong vector definition");

  s = lexer->get_next_token();
  if (s.type == Token::VAL)
    y = s.val;
  else if (s.type == Token::IND)
    y = s.ind;
  else
    throw std::runtime_error("Wrong vector definition");

  s = lexer->get_next_token();
  if (s.type == Token::VAL)
    z = s.val;
  else if (s.type == Token::IND)
    z = s.ind;
  else
    throw std::runtime_error("Wrong vector definition");

  s = lexer->get_next_token();
  if (s.type == Token::ENDL || s.type == Token::ENDF)
    return glm::vec3(x, y, z);
  else
    throw std::runtime_error("Wrong vector definition");
}

Scene *SceneParser::loadScene(std::string path)
{
  SceneParser parser;
  parser.parse(path);
  return parser.getScene();
}