%{
	#include "node.h"
	#include <string>
	#include <iostream>

	Blk *progBlk;
	extern int yylex();
	void yyerror(char const *s){
		std::cout << std::string{s} << std::endl;
	}
%}


%define parser_class_name {Parser}
%define api.namespace {bison}

%union{
	Node *node;
	std::vector<Extern*> *externs;
	std::vector<FuncDecl*> *funcs;
	Blk *blk;
	Stmt *stmt;
	std::vector<Exp*> *exps;
	Exp *exp;
	std::vector<*VarDecl> vdecls;
	std::vector<*Type> tdecls;
}

%destructor{
	if(&&){
		delete (&&;
		(&&) = nullptr;
	}
}

%token DEF "def"
%token EXT "extern"
%token <str> IDENT
%token <num> NUM

%token <op> ASSIGN "="
%token <op> EQ "=="
%token <op> NEQ "!="
%token <op> NOT "!"
%token <op> MUL "*"
%token <op> DIV "/"
%token <op> ADD "+"
%token <op> SUB "-"
%token <op> GT ">"
%token <op> LT "<"
%token <op> AND "&&"
%token <op> OR "||"

%token <type> INT "int"
%token <type> CINT "cint"
%token <type> FLOAT "float"
%token <type> SFLOAT "sfloat"
%token <type> VOID "void"

%token IF "if"
%token ELSE "else"
%token FOR "for"
%token REF "ref"
%token VAR "&"


%start prog

%%

prog : 
	externs funcs {$$ = new Prog($1, $2);}
|	funcs {$$ = new Prog(nullptr, $1);}

externs :
	externs extern {$$->push_back($2);}
|	extern {$$ = new std::vector<Extern*>(); $$->push_back($1);}

extern :
	EXT type globid LPAREN tdecls RPAREN {$$ = new Extern($1, $2, $4);}
|	EXT type globid LPAREN RPAREN {$$ = new Extern($1, $2);}

funcs :
	funcs func {$1->push_back($2);}
|	func {$$ = new std::vector<FuncDecl*>(); $$->push_back($1)}

func :
	DEF type globid LPAREN vdecls RPAREN blk {$$ = new FuncDecl(*$2, *$3, *$5, *$7);}
|	DEF type globid LPAREN RPAREN blk{$$ = new FuncDecl(*$2, *$3, *$6);}

blk :
	LBRACE stmts RBRACE {$$ = new Blk($2);}
|	LBRACE RBRACE {$$ = new Blk();}

stmts :
	stmts stmt {$1->statements.push_back($2);}
|	stmt {$$ = new Blk(); $$->statements.push_back($1);}

stmt :
	blk
|	RET exp
|	RET
|	vdecl "=" exp
|	exp
|	WHILE LPAREN exp RPAREN stmt
|	IF LPAREN exp RPAREN ELSE stmt
|	IF LPAREN exp RPAREN
|	PRINT exp
|	PRINT slit

exps :
	exps COMMA exp {$1->push_back($2);}
|	exp {$$ = new std::vector<Exp*>(); $$->push_back($1);}

exp :
	LPAREN exp RPAREN {$$ = $2;}
|	binop
|	uop
|	lit
|	var
|	globid LPAREN exps RPAREN {$$ = new FuncCall(*$1, *$3); delete $3;}
|	globid LPAREN RPAREN {$$ = new FuncCall(*$1);}

%left "="
%left "||"
%left "&&"
%left "=="
%left ">" "<"
%left "+" "-"
%left "*" "/"

binop :
	exp <op> exp {$$ = new BinOp(*$1, *$2, *$3);}

uop :
	"!" exp {$$ = new UaryOp(*$1, *$2);}
|	"-" exp {$$ = new UaryOp(*$1, *$2);}

lit :
	NUM {$$ = new Num(*$1);}

slit :
	STR {$$ = new Str{*$1};}

ident :
	IDENT {$$ = new Ident{*$1};}

var :
	VAR ident {$$ = new Var(*$2);}

globid :
	ident {$$ = new Globid(*$1);}

type :
	<type> {$$ = new Type{$1};}
|	<type> REF {$$ = new Type{$2, true};}


vdecls :
	vdecls COMMA vdecl {$1->push_back($3);}
|	vdecl {$$ = new std::vector<*VarDecl>(); $$->push_back($1);}

tdecls :
	types COMMA type {$1->push_pack($3);}
|	type {$$ = new std::vector<*Type>(); $$->push_back($1);}

vdecl :
	type var {$$ = new VarDecl($1, $2);}

%%
