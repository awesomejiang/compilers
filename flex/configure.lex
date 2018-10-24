%{
#include "node.h"
#include "parser.hpp"
#include <string>

%}

%%

[ \t\n]					;
[a-zA-Z_][a-zA-Z0-9_]*	return IDENT;
[0-9]+\.[0-9]*			return FLOAT;
[0-9]+					return INT;
"def"					return DEF;
"extern"				return EXT;
"$"						return VAR;
"ref"					return REF;
"="						return ASSIGN;
"=="					return CEQ;
"!="					return NEQ;
"!"						return NOT;
"<"						return LT;
">"						return GT;
"."						return DOT;
","						return COMMA;
"+"						return PLUS
"-"						return MINUS;
"*"						return MUL;
"/"						return DIV;
"("						return LPAREN;
")"						return RPAREN;
"{"						return LBRACE;
"}"						return RBRACE;
.						std::cout << "error: Unknown token!" << std::endl;


%%