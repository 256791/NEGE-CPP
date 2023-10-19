#include "ObjParser.hpp"
#include "ObjLexer.hpp"

#include <fstream>
#include <vector>
#include <filesystem>

#include "MtlParser.hpp"

#define GET_NAME(dest, lexer, error) \
  dest = lexer->get_next_token();    \
  if (dest.type != Token::NAME)      \
    throw std::runtime_error(error);

ObjParser::Symbol::Symbol(Token type) : type(type){};
ObjParser::Symbol::Symbol(float val) : type(Token::VAL), val(val){};
ObjParser::Symbol::Symbol(int ind) : type(Token::IND), ind(ind){};
ObjParser::Symbol::Symbol(std::string name) : type(Token::NAME), name(name){};
ObjParser::Symbol::Symbol(Token type, std::string name) : type(type), name(name){};

ObjParser::ObjParser() { lexer = new ObjLexer(); }
ObjParser::~ObjParser() { delete lexer; }

void ObjParser::parse(std::string path)
{
  std::ifstream file(path);
  if (!file.is_open())
    throw std::runtime_error("Can not open obj file");
  lexer->switch_streams(&file, NULL);

  std::vector<std::string> materialLibs;

  objects.clear();
  objects.push_back(Obj());

  std::string dir = path.substr(0, path.find_last_of('\\'));
  while (true)
  {
    Symbol current = lexer->get_next_token();
    switch (current.type)
    {
    case Token::V:
      parseVertex();
      break;
    case Token::VT:
      parseUV();
      break;
    case Token::VN:
      parseNormal();
      break;
    case Token::F:
      parseFace();
      break;
    case Token::ENDL:
      break;
    case Token::O:
      current = lexer->get_next_token();
      objects.push_back(Obj());

      if (current.type == Token::ENDL)
        objects.back().name = "undefined";
      else if (current.type == Token::NAME)
      {
        objects.back().name = current.name;
        current = lexer->get_next_token();
        if (current.type != Token::ENDL)
          throw std::runtime_error("Wrong Object definition");
      }
      else
        throw std::runtime_error("Wrong Object definition");

      break;
    case Token::G:
      current = lexer->get_next_token();
      objects.back().groups.push_back(Obj::Group());

      if (current.type == Token::ENDL)
        objects.back().groups.back().name = "undefined";
      else if (current.type == Token::NAME)
      {
        objects.back().groups.back().name = current.name;
        current = lexer->get_next_token();
        if (current.type != Token::ENDL)
          throw std::runtime_error("Wrong Object definition");
      }
      else
        throw std::runtime_error("Wrong Object definition");
      break;
    case Token::MTLLIB:
      GET_NAME(current, lexer, "Wrong material source definition")
      materialLibs.push_back(current.name);
      break;
    case Token::USEMTL:
      GET_NAME(current, lexer, "Wrong material use definition")
      objects.back().groups.back().materialName = current.name;
      break;
    case Token::ENDF:
      for (std::string matSrc : materialLibs)
      {
        MtlParser parser;
        parser.parse(std::filesystem::path(path).replace_filename(matSrc).string());
        std::vector<std::shared_ptr<Material>> tmp = parser.getMaterials();
        materials.insert(materials.end(), tmp.begin(), tmp.end());
      }
      return;
      break;
    default:
#ifdef DEBUG
      std::cerr << "Warning ignoring unsupported statement inside obj file " << current.type << "\n";
#endif
      break;
    }
  }
}

std::vector<std::shared_ptr<Model>> ObjParser::getModels()
{
  std::vector<std::shared_ptr<Model>> models;
  for (Obj obj : objects)
  {
    if (obj.vertices.size() > 0)
      models.push_back(obj.toModel(materials));
  }
  return models;
}

void ObjParser::parseVertex()
{
  float x, y, z, w;

  Symbol s = lexer->get_next_token();
  if (s.type == Token::VAL)
    x = s.val;
  else if (s.type == Token::IND)
    x = s.ind;
  else
    throw std::runtime_error("Wrong Vertex definition");

  s = lexer->get_next_token();
  if (s.type == Token::VAL)
    y = s.val;
  else if (s.type == Token::IND)
    y = s.ind;
  else
    throw std::runtime_error("Wrong Vertex definition");

  s = lexer->get_next_token();
  if (s.type == Token::VAL)
    z = s.val;
  else if (s.type == Token::IND)
    z = s.ind;
  else
    throw std::runtime_error("Wrong Vertex definition");

  s = lexer->get_next_token();
  if (s.type == Token::ENDL || s.type == Token::ENDF)
  {
    objects.back().vertices.push_back(glm::vec4(x, y, z, 1.f));
    return;
  }
  else if (s.type == Token::VAL)
    w = s.val;
  else if (s.type == Token::IND)
    w = s.ind;
  else
    throw std::runtime_error("Wrong Vertex definition");

  s = lexer->get_next_token();
  if (s.type == Token::ENDL || s.type == Token::ENDF)
    objects.back().vertices.push_back(glm::vec4(x, y, z, w));
  else
    throw std::runtime_error("Wrong Vertex definition");
}

void ObjParser::parseUV()
{
  float x, y, z;

  Symbol s = lexer->get_next_token();
  if (s.type == Token::VAL)
    x = s.val;
  else if (s.type == Token::IND)
    x = s.ind;
  else
    throw std::runtime_error("Wrong UV definition");

  s = lexer->get_next_token();
  if (s.type == Token::VAL)
    y = s.val;
  else if (s.type == Token::IND)
    y = s.ind;
  else
    throw std::runtime_error("Wrong UV definition");

  s = lexer->get_next_token();
  if (s.type == Token::ENDL || s.type == Token::ENDF)
  {
    objects.back().uvs.push_back(glm::vec3(x, y, 0.f));
    return;
  }
  else if (s.type == Token::VAL)
    z = s.val;
  else if (s.type == Token::IND)
    z = s.ind;
  else
    throw std::runtime_error("Wrong UV definition");

  s = lexer->get_next_token();
  if (s.type == Token::ENDL || s.type == Token::ENDF)
    objects.back().uvs.push_back(glm::vec3(x, y, z));
  else
    throw std::runtime_error("Wrong UV definition");
}

void ObjParser::parseNormal()
{
  float x, y, z;

  Symbol s = lexer->get_next_token();
  if (s.type == Token::VAL)
    x = s.val;
  else if (s.type == Token::IND)
    x = s.ind;
  else
    throw std::runtime_error("Wrong UV definition");

  s = lexer->get_next_token();
  if (s.type == Token::VAL)
    y = s.val;
  else if (s.type == Token::IND)
    y = s.ind;
  else
    throw std::runtime_error("Wrong UV definition");

  s = lexer->get_next_token();
  if (s.type == Token::VAL)
    z = s.val;
  else if (s.type == Token::IND)
    z = s.ind;
  else
    throw std::runtime_error("Wrong UV definition");

  s = lexer->get_next_token();
  if (s.type == Token::ENDL || s.type == Token::ENDF)
    objects.back().normals.push_back(glm::vec3(x, y, z));
  else
    throw std::runtime_error("Wrong UV definition");
}

void ObjParser::parseFace()
{
  std::vector<int> buff;
  int sepcount = 0;
  while (true)
  {
    Symbol s = lexer->get_next_token();
    if (s.type == Token::ENDL || s.type == Token::ENDF)
      break;
    if (s.type == Token::SEP)
      sepcount++;
    else if (s.type == Token::IND)
      buff.push_back(s.ind);
    else
      throw std::runtime_error("Wrong Face definition");
  }

  if (buff.size() == 3 && sepcount == 0)
  {
    objects.back().groups.back().faces.push_back(
        Obj::Face(
            glm::ivec3(buff[0], buff[1], buff[2])));
  }
  else if (buff.size() == 6 && sepcount == 3)
  {
    objects.back().groups.back().faces.push_back(
        Obj::Face(
            glm::ivec3(buff[0], buff[2], buff[4]),
            glm::ivec3(buff[1], buff[3], buff[5])));
  }
  else if (buff.size() == 6 && sepcount == 6)
  {
    objects.back().groups.back().faces.push_back(
        Obj::Face(
            glm::ivec3(buff[0], buff[2], buff[4]),
            glm::ivec3(0),
            glm::ivec3(buff[1], buff[3], buff[5])));
  }
  else if (buff.size() == 9 && sepcount == 6)
  {
    objects.back().groups.back().faces.push_back(
        Obj::Face(
            glm::ivec3(buff[0], buff[3], buff[6]),
            glm::ivec3(buff[1], buff[4], buff[7]),
            glm::ivec3(buff[2], buff[5], buff[8])));
  }
  else if (buff.size() == 4 && sepcount == 0)
  {
    objects.back().groups.back().faces.push_back(
        Obj::Face(
            glm::ivec3(buff[0], buff[1], buff[2])));

    objects.back().groups.back().faces.push_back(
        Obj::Face(
            glm::ivec3(buff[0], buff[2], buff[3])));
  }
  else if (buff.size() == 8 && sepcount == 4)
  {
    objects.back().groups.back().faces.push_back(
        Obj::Face(
            glm::ivec3(buff[0], buff[2], buff[4]),
            glm::ivec3(buff[1], buff[3], buff[5])));
    objects.back().groups.back().faces.push_back(
        Obj::Face(
            glm::ivec3(buff[0], buff[4], buff[6]),
            glm::ivec3(buff[1], buff[5], buff[7])));
  }
  else if (buff.size() == 8 && sepcount == 8)
  {
    objects.back().groups.back().faces.push_back(
        Obj::Face(
            glm::ivec3(buff[0], buff[2], buff[4]),
            glm::ivec3(0),
            glm::ivec3(buff[1], buff[3], buff[5])));
    objects.back().groups.back().faces.push_back(
        Obj::Face(
            glm::ivec3(buff[0], buff[4], buff[6]),
            glm::ivec3(0),
            glm::ivec3(buff[1], buff[5], buff[7])));
  }
  else if (buff.size() == 12 && sepcount == 8)
  {
    objects.back().groups.back().faces.push_back(
        Obj::Face(
            glm::ivec3(buff[0], buff[3], buff[6]),
            glm::ivec3(buff[1], buff[4], buff[7]),
            glm::ivec3(buff[2], buff[5], buff[8])));
    objects.back().groups.back().faces.push_back(
        Obj::Face(
            glm::ivec3(buff[0], buff[6], buff[9]),
            glm::ivec3(buff[1], buff[7], buff[10]),
            glm::ivec3(buff[2], buff[8], buff[11])));
  }
  else
    throw std::runtime_error("Wrong Face definition (only triangles and quads supported)");
}

std::vector<std::shared_ptr<Model>> ObjParser::loadModels(std::string path){
  ObjParser parser;
  parser.parse(path);
  return parser.getModels();
}