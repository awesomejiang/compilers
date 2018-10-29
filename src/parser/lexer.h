#pragma once

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL int Lexer::yylex()

#include "parser.hpp"

class Lexer: public yyFlexLexer{
public:
	Lexer(std::istream *in): yyFlexLexer{in} {}

	int yylex(bison::Parser::semantic_type *l_val){
		yylval = l_val;
		return yylex();
	}

private:
	int yylex();
	bison::Parser::semantic_type *yylval = nullptr;
};