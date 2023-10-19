%option c++ nodefault noyywrap
%option yyclass="ObjLexer"
%option prefix="obj"
%option yylineno

%{
	#include <iostream>
	#include <cstdlib>
	#include "ObjLexer.hpp"
	#include "ObjParser.hpp"
%}

%x COMMENT

%%

"#"      			{BEGIN(COMMENT);}
<COMMENT>.		;
<COMMENT>\n		{BEGIN(INITIAL);}

"o "        {return(ObjParser::Symbol(ObjParser::Token::O));}
"g "        {return(ObjParser::Symbol(ObjParser::Token::G));}

"v "        {return(ObjParser::Symbol(ObjParser::Token::V));}
"vt "       {return(ObjParser::Symbol(ObjParser::Token::VT));}
"vn "       {return(ObjParser::Symbol(ObjParser::Token::VN));}
"vp "       {return(ObjParser::Symbol(ObjParser::Token::VP));}

"f "        {return(ObjParser::Symbol(ObjParser::Token::F));}
"p "        {return(ObjParser::Symbol(ObjParser::Token::P));}
"l "        {return(ObjParser::Symbol(ObjParser::Token::L));}
"/"         {return(ObjParser::Symbol(ObjParser::Token::SEP));}

"usemtl "		{return(ObjParser::Symbol(ObjParser::Token::USEMTL));}
"mtllib "		{return(ObjParser::Symbol(ObjParser::Token::MTLLIB));}


-?[0-9]+            	{return(ObjParser::Symbol(std::stoi(yytext)));}
-?[0-9]+\.[0-9]+    	{return(ObjParser::Symbol(std::stof(yytext)));}
[_A-Za-z]+[A-Za-z0-9_\.]*             {return(ObjParser::Symbol(std::string(yytext)));}

\n     				{return(ObjParser::Symbol(ObjParser::Token::ENDL));}

"parm "       {return(ObjParser::Symbol(ObjParser::Token::PARM));}
"trim "       {return(ObjParser::Symbol(ObjParser::Token::TRIM));}
"hole "       {return(ObjParser::Symbol(ObjParser::Token::HOLE));}
"scrv "       {return(ObjParser::Symbol(ObjParser::Token::SCRV));}
"sp "        	{return(ObjParser::Symbol(ObjParser::Token::SP));}
"end "        {return(ObjParser::Symbol(ObjParser::Token::END));}
"s "        	{return(ObjParser::Symbol(ObjParser::Token::S));}

[\r\t ]				{/*ingore*/}
.							{return(ObjParser::Symbol(ObjParser::Token::ERR, "Unrecognized symbol:" + std::string(yytext)));}

<<EOF>>     	{return(ObjParser::Symbol(ObjParser::Token::ENDF));}

%%