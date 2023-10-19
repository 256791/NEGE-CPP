%option c++ nodefault noyywrap
%option yyclass="SceneLexer"
%option prefix="scene"
%option yylineno

%{
	#include <iostream>
	#include <cstdlib>
	#include "SceneLexer.hpp"
	#include "SceneParser.hpp"
%}

%x COMMENT

%%

"#"      			{BEGIN(COMMENT);}
<COMMENT>.		;
<COMMENT>\n		{BEGIN(INITIAL);}

"E3"							{return(SceneParser::Symbol(SceneParser::Token::E3));}
"H3"							{return(SceneParser::Symbol(SceneParser::Token::H3));}
"H3LOCK"					{return(SceneParser::Symbol(SceneParser::Token::H3LOCK));}

"MODEL " 					{return(SceneParser::Symbol(SceneParser::Token::MODEL));}
"ENTITY " 				{return(SceneParser::Symbol(SceneParser::Token::ENTITY));}

"T "							{return(SceneParser::Symbol(SceneParser::Token::T));}
"S "							{return(SceneParser::Symbol(SceneParser::Token::S));}
"R " 							{return(SceneParser::Symbol(SceneParser::Token::R));}

-?[0-9]+            	{return(SceneParser::Symbol(std::stoi(yytext)));}
-?[0-9]+\.[0-9]+    	{return(SceneParser::Symbol(std::stof(yytext)));}
[_\.A-Za-z]+[A-Za-z0-9_\.]*            {return(SceneParser::Symbol(std::string(yytext)));}

\n     				{return(SceneParser::Symbol(SceneParser::Token::ENDL));}

[\r\t ]				{/*ingore*/}
.							{return(SceneParser::Symbol(SceneParser::Token::ERR, "Unrecognized symbol:" + std::string(yytext)));}

<<EOF>>     	{return(SceneParser::Symbol(SceneParser::Token::ENDF));}

%%