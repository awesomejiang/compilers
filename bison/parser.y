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

%union{
	Node *node;
	Blk *blk;
	Exp *exp;
	Stmt *stmt;
	Ident *ident;
	VarDecl *varDecl;
	std::vector *varVec;
	std::vector *expVec;
	std::string *string;
	int token;
}

%token IDENT INT FLOAT
%token EQ NE LT LE GT GE ASSIGN
%token LPAREN RPAREN LBRACE RBRACE COMMA DOT
%token PLUS MINUS MUL DIV

%type ident
%type numeric exp
%type funcDeclArgs
%type callArgs
%type prog stmts blk
%type stmt varDecl funcDecl
%type comparison

%left PLUS MINUS
%left MUL DIV

%start prog

%%

prog : externDecl prog {$$ = new ExternDecl($1);}
	 | func {}

stmts : stmt {$$ = new Blk(); $$->statements.push_back($1);}
	  | stmts stmt {$1->statements.push_back($2);}
	  ;

stmt : varDecl | funcDecl
	 | exp {$$ = new ExpStmt(*$1);}
	 ;

blk : LBRACE stmts RBRACE {$$ = $2;}
	| LBRACE RBRACE {$$ = new Blk();}
	;

varDecl : ident ident {$$ = new VarDecl(*$1, *$2);}
		 | ident ident ASSIGN exp {$$ = new Vdecl(*1, *2, *4);}
		 ;

funcDecl : ident ident LPAREN funcDeclArgs RPAREN block {$$ = new FuncDecl(*1, *2, *4, *6); delete $4;}
		  ;

funcDeclArgs : /*blank*/ {$$ = new VarList();}
			   | varDecl {$$ = new VarList(); $$->push_back($1);}
			   | funcDeclArgs COMMA varDecl {$1->push_back($3);}
			   ;

ident : IDENT {$$ = new Ident(*$1); delete $1;}
	  ;
	
numeric : INT {$$ = new Int(atol($1->c_str())); delete $1;}
		| FLOAT {$$ = new Float(atof($1->c_str())); delete $1;}
		;

exp : ident ASSIGN exp {$$ = new Assignment(*$1, *$3);}
	| ident LPAREN callArgs RPAREN {$$ = new MehtodCall(*$1, *$3); delete $3;}
	| ident {$$ = $1}
	| numeric
	| exp comparison exp {$$ = new BinOp(*$1, $2, *$3);}
	| LPAREN exp RPAREN {$$ =$2;}
	;

callArgs : /*blank*/ {$$ = new ExpList();}
		  | exp {$$ = new ExpList(); $$->push_back($1);}
		  | callArgs COMMA exp {$1->push_back($3);}
		  ;

comparison : EQ | NE | LT | LE | GT | GE | PLUS | MINUS | MUL | DIV
		   ;

%%












