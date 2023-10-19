#include "MtlParser.hpp"
#include "MtlLexer.hpp"

#include <fstream>
#include <vector>
#include <iostream>
// #include <sstream>

#define GET_NAME(error)              \
  current = lexer->get_next_token(); \
  if (current.type != Token::NAME)   \
    throw std::runtime_error(error);

#define GET_FLOAT(error)                                         \
  current = lexer->get_next_token();                             \
  if (current.type != Token::VAL && current.type != Token::IVAL) \
    throw std::runtime_error(error);

MtlParser::Symbol::Symbol(Token type) : type(type){};
MtlParser::Symbol::Symbol(Token type, float val) : type(type), val(val){};
MtlParser::Symbol::Symbol(std::string name) : type(MtlParser::Token::NAME), name(name){};

MtlParser::MtlParser() { lexer = new MtlLexer(); }
MtlParser::~MtlParser() { delete lexer; }

void MtlParser::parse(std::string path)
{
  std::ifstream file(path);
  if (!file.is_open())
    throw std::runtime_error("Can not open mtl file");
  lexer->switch_streams(&file, NULL);

  while (true)
  {
    Symbol current = lexer->get_next_token();
    switch (current.type)
    {
    case Token::NEWMTL:
      current = lexer->get_next_token();
      if (current.type != Token::NAME)
        throw std::runtime_error("Wrong Material definition");
      materials.push_back(std::shared_ptr<Material>(new Material(current.name)));
      break;
    case Token::KA:
      materials.back()->Ka = parseColor();
      break;
    case Token::KD:
      materials.back()->Kd = parseColor();
      break;
    case Token::KS:
      materials.back()->Ks = parseColor();
      break;
    case Token::MKA:
      materials.back()->map_Ka = parseMap();
      break;
    case Token::MKD:
      materials.back()->map_Kd = parseMap();
      break;
    case Token::MKS:
      materials.back()->map_Ks = parseMap();
      break;
    case Token::KE:
      break;
    case Token::NI:
      break;
    case Token::NS:
      current = lexer->get_next_token();
      if (current.type == Token::VAL || current.type == Token::IVAL)
        materials.back()->Ns = current.val;
      else
        throw std::runtime_error("Wrong ns definition");
      break;
    case Token::D:
      current = lexer->get_next_token();
      if (current.type == Token::VAL || current.type == Token::IVAL)
        materials.back()->d = current.val;
      else
        throw std::runtime_error("Wrong d definition");
      break;
    case Token::ILLUM:
      current = lexer->get_next_token();
      if (current.type == Token::IVAL)
        materials.back()->illum = current.val;
      else
        throw std::runtime_error("Wrong illum definition");
      break;
    case Token::VAL:
      break;
    case Token::IVAL:
      break;
    case Token::ENDL:
      break;
    case Token::ENDF:
      return;
      break;
    default:
#ifdef DEBUG
      std::cerr << "Warning ignoring unsupported statement inside mtl file " << current.type << "\n";
#endif
      break;
    }
  }
}

glm::vec3 MtlParser::parseColor()
{
  float x, y, z;

  Symbol s = lexer->get_next_token();
  if (s.type == Token::VAL || s.type == Token::IVAL)
    x = s.val;
  else
    throw std::runtime_error("Wrong RGB color definition");

  s = lexer->get_next_token();
  if (s.type == Token::VAL || s.type == Token::IVAL)
    y = s.val;
  else
    throw std::runtime_error("Wrong RGB color definition");

  s = lexer->get_next_token();
  if (s.type == Token::VAL || s.type == Token::IVAL)
    z = s.val;
  else
    throw std::runtime_error("Wrong RGB color definition");

  s = lexer->get_next_token();
  if (s.type == Token::ENDL || s.type == Token::ENDF)
    return glm::vec3(x, y, z);
  else
    throw std::runtime_error("Wrong RGB color definition");
}

std::shared_ptr<Texture> MtlParser::parseMap()
{
}

std::vector<std::shared_ptr<Material>> MtlParser::getMaterials()
{
  return materials;
}
