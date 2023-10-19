%option c++ nodefault noyywrap
%option yyclass="MtlLexer"
%option prefix="mtl"
%option yylineno

%{
	#include <iostream>
	#include <cstdlib>
	#include "MtlLexer.hpp"
	#include "MtlParser.hpp"
%}

%x COMMENT

%%

"#"      			{BEGIN(COMMENT);}
<COMMENT>.		;
<COMMENT>\n		{BEGIN(INITIAL);}

"newmtl " 				{return(MtlParser::Symbol(MtlParser::Token::NEWMTL));}

"Ka "     				{return(MtlParser::Symbol(MtlParser::Token::KA));}
"Kd "     				{return(MtlParser::Symbol(MtlParser::Token::KD));}
"Ks "     				{return(MtlParser::Symbol(MtlParser::Token::KS));}
"Ke "     				{return(MtlParser::Symbol(MtlParser::Token::KE));}

"Ns "     				{return(MtlParser::Symbol(MtlParser::Token::NS));}
"Ni "     				{return(MtlParser::Symbol(MtlParser::Token::NI));}

"d "     					{return(MtlParser::Symbol(MtlParser::Token::D));}

"map_Ka "     		{return(MtlParser::Symbol(MtlParser::Token::MKA));}
"map_Kd "     		{return(MtlParser::Symbol(MtlParser::Token::MKD));}
"map_Ks "     		{return(MtlParser::Symbol(MtlParser::Token::MKS));}

"illum "     			{return(MtlParser::Symbol(MtlParser::Token::ILLUM));}


-?[0-9]+            	{return(MtlParser::Symbol(MtlParser::Token::IVAL, std::stoi(yytext)));}
-?[0-9]+\.[0-9]+    	{return(MtlParser::Symbol(MtlParser::Token::VAL, std::stof(yytext)));}
[_A-Za-z]+[A-Za-z0-9_\.]*            {return(MtlParser::Symbol(std::string(yytext)));}

\n     				{return(MtlParser::Symbol(MtlParser::Token::ENDL));}


[\r\t ]				{/*ingore*/}
.							{return(MtlParser::Symbol(MtlParser::Token::ERR));}

<<EOF>>     	{return(MtlParser::Symbol(MtlParser::Token::ENDF));}

%%