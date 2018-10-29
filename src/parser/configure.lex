%{
#include "parser.hpp"
#include "node.h"
#include "lexer.h"

#include <iostream>
#include <string>

using token = bison::Parser::token;


#define SAVE_TOKEN yylval->str = new std::string(yytext, yyleng)

%}

%option c++
%option yyclass="Lexer"
%option noyywrap

%%

"extern"				return token::EXT;
"def"					return token::DEF;
"return"				return token::RET;
"if"					return token::IF;
"else"					return token::ELSE;
"while"					return token::WHILE;
"print"					return token::PRINT;
"ref"					return token::REF;
"noalias"				return token::NOALIAS;
"$"						return token::VAR;

"."						return token::DOT;
","						return token::COMMA;
";"						return token::END;
"("						return token::LPAREN;
")"						return token::RPAREN;
"{"						return token::LBRACE;
"}"						return token::RBRACE;

"="						return token::ASSIGN;
"=="					return token::EQ;
"!="					return token::NEQ;
"!"						return token::NOT;
"<"						return token::LT;
">"						return token::GT;
"+"						return token::ADD;
"-"						return token::SUB;
"*"						return token::MUL;
"/"						return token::DIV;
"&&"					return token::AND;
"||"					return token::OR;

"int"					return token::INT;
"cint"					return token::CINT;
"float"					return token::FLOAT;
"sfloat"				return token::SFLOAT;
"void"					return token::VOID;

#[^\n]*\n				;
[ \t\n]					;
[a-zA-Z_][a-zA-Z0-9_]*	SAVE_TOKEN; return token::IDENT;
[0-9]+(\.[0-9]+)?		SAVE_TOKEN; return token::NUM;
\"[^"]*\"				SAVE_TOKEN; return token::STR;

.						std::cout << "error: Unknown token!" << std::endl;


%%