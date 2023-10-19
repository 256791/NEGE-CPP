#ifndef OBJ_LEXER_HPP
#define OBJ_LEXER_HPP

#ifndef yyFlexLexerOnce
#undef yyFlexLexer
#define yyFlexLexer objFlexLexer
#include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL ObjParser::Symbol ObjLexer::get_next_token()


#include "ObjParser.hpp"

class ObjLexer : public yyFlexLexer
{
public:
    ObjLexer() {}
    virtual ~ObjLexer() {}
    virtual ObjParser::Symbol get_next_token();
};
#endif