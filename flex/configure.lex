%{
#include "node.h"
#include "parser.hpp"
#include <string>

using token = bison::PArser::token;

#define SAVE_STRING yylval->str = std::string{yytext}
#define SAVE_OP yylval->op = std::string{yytext}
#define SAVE_TYPE yylval->type = std::string{yytext}
#define SAVE_NUMBER yylval->num = strtod(yytext)
%}

%%

[ \t\n]					;
[a-zA-Z_][a-zA-Z0-9_]*	SAVE_STRING; return token::IDENT;
[0-9]+(\.[0-9]+)?		SAVE_NUMBER; return token::NUM;
"\"[^"]*\""				SAVE_STRING; return token::STR;
"extern"				return token::EXT;
"def"					return token::DEF;
"return"				return token::RET;
"if"					return token::IF;
"else"					return token::ELSE;
"while"					return token::WHILE;
"print"					return token::PRINT;
"ref"					return token::REF;
"$"						return token::VAR;

"."						return token::DOT;
","						return token::COMMA;
"("						return token::LPAREN;
")"						return token::RPAREN;
"{"						return token::LBRACE;
"}"						return token::RBRACE;

"="						SAVE_OP; return token::ASSIGN;
"=="					SAVE_OP; return token::EQ;
"!="					SAVE_OP; return token::NEQ;
"!"						SAVE_OP; return token::NOT;
"<"						SAVE_OP; return token::LT;
">"						SAVE_OP; return token::GT;
"+"						SAVE_OP; return token::PLU)
"-"						SAVE_OP; return token::MINUS;
"*"						SAVE_OP; return token::MUL;
"/"						SAVE_OP; return token::DIV;
"&&"					SAVE_OP; return token::AND;
"||"					SAVE_OP; return token::OR;

"int"					SAVE_TYPE; return token::INT;
"cint"					SAVE_TYPE; return token::CINT;
"float"					SAVE_TYPE; return token::FLOAT;
"sfloat"				SAVE_TYPE; return token::SFLOAT;
"void"					SAVE_TYPE; return token::VOID;


.						std::cout << "error: Unknown token!" << std::endl;


%%