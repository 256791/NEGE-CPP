#ifndef MTL_LEXER_HPP
#define MTL_LEXER_HPP

#ifndef yyFlexLexerOnce
#undef yyFlexLexer
#define yyFlexLexer mtlFlexLexer
#include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL MtlParser::Symbol MtlLexer::get_next_token()


#include "MtlParser.hpp"

class MtlLexer : public yyFlexLexer
{
public:
    MtlLexer() {}
    virtual ~MtlLexer() {}
    virtual MtlParser::Symbol get_next_token();
};
#endif