#ifndef OBJ_PARSER_HPP
#define OBJ_PARSER_HPP

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "Obj.hpp"
#include "../components/Material.hpp"


class ObjLexer;

// Simple Parser instead bison
class ObjParser
{
public:
  ObjParser();
  ~ObjParser();
  void parse(std::string path);
  std::vector<std::shared_ptr<Model>> getModels();
  static std::vector<std::shared_ptr<Model>> loadModels(std::string path);

protected:
  friend class ObjLexer;
  enum Token
  {
    G, O, S,
    V, VT, VN, VP,
    P, L, F, SEP,
    PARM, TRIM, HOLE, SCRV, SP, END,
    VAL, IND, NAME,
    MTLLIB, USEMTL,
    ENDL, ENDF,ERR
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
  ObjLexer *lexer;
  std::vector<Obj> objects;
  std::vector<std::shared_ptr<Material>> materials;
  
  void parseVertex();
  void parseNormal();
  void parseUV();
  void parseFace();
};
#endif