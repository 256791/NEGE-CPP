#ifndef MTL_PARSER_HPP
#define MTL_PARSER_HPP

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "../components/Material.hpp"
#include "../components/Texture.hpp"

class MtlLexer;

class MtlParser
{
public:
  MtlParser();
  ~MtlParser();
  void parse(std::string path);
  std::vector<std::shared_ptr<Material>> getMaterials();

protected:
  friend class MtlLexer;
  enum Token
  {
    NEWMTL,
    KA, KD, KS, KE, 
    MKA, MKD, MKS, 
    NS, NI, D, ILLUM,
    VAL, IVAL, NAME,
    ENDL, ENDF, ERR
  };

  struct Symbol
  {
    Symbol(Token type);
    Symbol(Token type, float val);
    Symbol(std::string name);

    Token type;
    float val;
    std::string name;
  };

private:
  MtlLexer *lexer;
  glm::vec3 parseColor();
  std::shared_ptr<Texture> parseMap();
  std::vector<std::shared_ptr<Material>> materials;

};
#endif