%skeleton "lalr1.cc"

%debug

%defines
%define parser_class_name {Parser}
%define api.namespace {bison}
%define parse.error verbose

%param {Lexer &lexer}
%parse-param {Prog &prog}

%code requires{
	#include "node.h"
	#include <string>
	#include <iostream>

	class Lexer;
}

%code{
	#include "node.h"
	Prog *rootProg;

	static int yylex(bison::Parser::semantic_type *yylval,
	Lexer &lexer);
}

%union{
	Prog *progType;
	Stmt *stmtType;
	Blk *blkType;
	Extern *externType;
	ExternList *externsType;
	FuncDecl *funcdeclType;
	FuncList *funcsType;
	Exp *expType;
	ExpList *expsType;
	VarDecl *vdeclType;
	VarList *vdeclsType;
	NType *tdeclType;
	TypeList *tdeclsType;
	Globid *globidType;
	Var *varType;
	Str *strType;
	string *str;
}


%define api.token.prefix {}

%token <str> IDENT
%token <str> STR
%token <str> NUM

%token <str> ASSIGN "="
%token <str> EQ "=="
%token <str> NEQ "!="
%token <str> NOT "!"
%token <str> MUL "*"
%token <str> DIV "/"
%token <str> ADD "+"
%token <str> SUB "-"
%token <str> GT ">"
%token <str> LT "<"
%token <str> AND "&&"
%token <str> OR "||"

%token <str> INT "int"
%token <str> CINT "cint"
%token <str> FLOAT "float"
%token <str> SFLOAT "sfloat"
%token <str> VOID "void"

%token DEF "def"
%token EXT "extern"

%token IF "if"
%token ELSE "else"
%token FOR "for"
%token WHILE "while"
%token PRINT "print"
%token RET "return"
%token NOALIAS "noalias"
%token REF "ref"
%token VAR "&"

%token LPAREN "("
%token RPAREN ")"
%token LBRACE "{"
%token RBRACE "}"
%token COMMA ","
%token DOT "."
%token END ";"

%type <progType> prog
%type <stmtType> stmt
%type <blkType> blk stmts
%type <externType> extern
%type <externsType> externs
%type <funcdeclType> func
%type <funcsType> funcs
%type <expType> exp binop uop lit
%type <expsType> exps
%type <vdeclType> vdecl
%type <vdeclsType> vdecls
%type <tdeclType> type
%type <tdeclsType> tdecls
%type <globidType> globid
%type <varType> var
%type <strType> slit

%%
%start prog;

prog : 
	externs funcs {$$ = new Prog($1, $2); rootProg = $$;}
|	funcs {$$ = new Prog(nullptr, $1); rootProg = $$;}
|	{/* empty file */}	

externs :
	externs extern {$1->push_back($2); $$ = $1;}
|	extern {$$ = new ExternList(); $$->push_back($1);}


extern :
	EXT type globid LPAREN tdecls RPAREN END {$$ = new Extern($2, $3, $5);}
|	EXT type globid LPAREN RPAREN END {$$ = new Extern($2, $3);}

funcs :
	funcs func {$1->push_back($2); $$ = $1;}
|	func {$$ = new FuncList(); $$->push_back($1);}

func :
	DEF type globid LPAREN vdecls RPAREN blk {$$ = new FuncDecl($2, $3, $7, $5);}
|	DEF type globid LPAREN RPAREN blk {$$ = new FuncDecl($2, $3, $6);}

blk :
	LBRACE stmts RBRACE {$$ = new Blk(*$2);}
|	LBRACE RBRACE {$$ = new Blk();}

stmts :
	stmts stmt {$1->statements.push_back($2); $$ = $1;}
|	stmt {$$ = new Blk(); $$->statements.push_back($1);}

stmt :
	blk {$$ = $1;}
|	RET exp END {$$ = new ReturnStmt($2);}
|	RET END {$$ = new ReturnStmt();}
|	vdecl "=" exp END {$$ = new AssignStmt($1, $3);}
|	exp END {$$ = new ExpStmt($1);}
|	WHILE LPAREN exp RPAREN stmt {$$ = new WhileStmt($3, $5);}
|	IF LPAREN exp RPAREN stmt ELSE stmt {$$ = new IfStmt($3, $5, $7);}
|	IF LPAREN exp RPAREN stmt {$$ = new IfStmt($3, $5);}
|	PRINT exp END {$$ = new PrintStmt($2);}
|	PRINT slit END {$$ = new PrintSlitStmt($2);}

exps :
	exps COMMA exp {$1->push_back($3); $$ = $1;}
|	exp {$$ = new ExpList(); $$->push_back($1);}

exp :
	LPAREN exp RPAREN {$$ = $2;}
|	binop {$$ = $1;}
|	uop {$$ = $1;}
|	lit {$$ = $1;}
|	var {$$ = $1;}
|	globid LPAREN exps RPAREN {$$ = new FuncCall($1, $3);}
|	globid LPAREN RPAREN {$$ = new FuncCall($1);}

%left "=";
%left "||";
%left "&&";
%left "==";
%left ">" "<";
%left "+" "-";
%left "*" "/";

binop :
	exp "=" exp {$$ = new BinOp($1, "=", $3);}
|	exp "||" exp {$$ = new BinOp($1, "||", $3);}
|	exp "&&" exp {$$ = new BinOp($1, "&&", $3);}
|	exp "==" exp {$$ = new BinOp($1, "==", $3);}
|	exp ">" exp {$$ = new BinOp($1, ">", $3);}
|	exp "<" exp {$$ = new BinOp($1, "<", $3);}
|	exp "+" exp {$$ = new BinOp($1, "+", $3);}
|	exp "-" exp {$$ = new BinOp($1, "-", $3);}
|	exp "*" exp {$$ = new BinOp($1, "*", $3);}
|	exp "/" exp {$$ = new BinOp($1, "/", $3);}

uop :
	"!" exp {$$ = new UaryOp("!", $2);}
|	"-" exp {$$ = new UaryOp("-", $2);}

lit :
	NUM {$$ = new Num(*$1);}

slit :
	STR {$$ = new Str{*$1};}

var :
	VAR IDENT {$$ = new Var(*$2);}

globid :
	IDENT {$$ = new Globid(*$1);}

type :
	"int" {$$ = new NType{"int"};}
|	"cint" {$$ = new NType{"cint"};}
|	"float" {$$ = new NType{"float"};}
|	"sfloat" {$$ = new NType{"sfloat"};}
|	"void" {$$ = new NType{"void"};}
|	REF type {$2->setRef(); $$ = $2;}
|	NOALIAS type {$2->setNoalias(); $$ = $2;}

tdecls :
	tdecls COMMA type {$1->push_back($3); $$ = $1;}
|	type {$$ = new TypeList(); $$->push_back($1);}

vdecls :
	vdecls COMMA vdecl {$1->push_back($3); $$ = $1;}
|	vdecl {$$ = new VarList(); $$->push_back($1);}

vdecl :
	type var {$$ = new VarDecl($1, $2);}

%%

#include "lexer.h"

static int yylex(bison::Parser::semantic_type *yylval,
                 Lexer &lexer) {
    return lexer.yylex(yylval);
}

void
bison::Parser::error( const string &err_message )
{
   std::cerr << "Error: " << err_message << "\n";
}