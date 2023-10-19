#ifndef OBJ_LEXER_HPP
#define OBJ_LEXER_HPP

#ifndef yyFlexLexerOnce
#undef yyFlexLexer
#define yyFlexLexer sceneFlexLexer
#include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL SceneParser::Symbol SceneLexer::get_next_token()


#include "SceneParser.hpp"

class SceneLexer : public yyFlexLexer
{
public:
    SceneLexer() {}
    virtual ~SceneLexer() {}
    virtual SceneParser::Symbol get_next_token();
};
#endif