#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

#include <yaml-cpp/yaml.h>

#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>

using std::vector;
using std::string;

class Exp;
class Stmt;
class VarDecl;
class Type;
class Extern;
class FuncDecl;

using ExpList = vector<Exp*>;
using StmtList = vector<Stmt*>;
using VarList = vector<VarDecl*>;
using TypeList = vector<Type*>;
using ExternList = vector<Extern*>;
using FuncList = vector<FuncDecl*>;

class Node{
public:
	virtual void printYaml(YAML::Emitter &out) {}
};

class Stmt: public Node{
public:
	virtual void printYaml(YAML::Emitter &out) {}
};

class Exp: public Node{
public:
	virtual void printYaml(YAML::Emitter &out) {}
};

class Num: public Exp{
public:
	Num(string const &str): val{str} {}
	virtual void printYaml(YAML::Emitter &out);
private:
	string val;
};


class Ident: public Exp{
public:
	Ident(string const &name): name{name} {}
	virtual void printYaml(YAML::Emitter &out) {}

	string name;
};

class Str: public Exp{
public:
	Str(string const &name): name{name.substr(1, name.size()-2)} {}
	virtual void printYaml(YAML::Emitter &out);

	string name;
};

class Var: public Ident{
public:
	using Ident::Ident;
	virtual void printYaml(YAML::Emitter &out);
};

class Globid: public Ident{
public:
	using Ident::Ident;
	virtual void printYaml(YAML::Emitter &out);
};


class ExpStmt: public Stmt{
public:
	ExpStmt(Exp* const exp): exp{exp} {}
	virtual void printYaml(YAML::Emitter &out);
private:
	Exp *exp;
};

class AssignStmt: public Stmt{
public:
	AssignStmt(VarDecl* const vdecl, Exp* const exp): vdecl{vdecl}, exp{exp} {}
	virtual void printYaml(YAML::Emitter &out);
private:
	VarDecl* vdecl;
	Exp *exp;
};

class ReturnStmt: public Stmt{
public:
	ReturnStmt(Exp* const exp = nullptr): exp{exp} {}
	virtual void printYaml(YAML::Emitter &out);
private:
	Exp *exp;
};

class WhileStmt: public Stmt{
public:
	WhileStmt(Exp* const exp, Stmt* const stmt): exp{exp}, stmt{stmt} {}
	virtual void printYaml(YAML::Emitter &out);
private:
	Exp *exp;
	Stmt *stmt;
};

class IfStmt: public Stmt{
public:
	IfStmt(Exp* const exp, Stmt* const stmt, Stmt* const elsestmt = nullptr)
	: exp{exp}, stmt{stmt}, elsestmt{elsestmt} {}
	virtual void printYaml(YAML::Emitter &out);
private:
	Exp *exp;
	Stmt *stmt, *elsestmt;
};

class PrintStmt: public Stmt{
public:
	PrintStmt(Exp* const exp): exp{exp} {}
	virtual void printYaml(YAML::Emitter &out);
private:
	Exp *exp;
};

class PrintSlitStmt: public Stmt{
public:
	PrintSlitStmt(Str* const str): str{str} {}
	virtual void printYaml(YAML::Emitter &out);
private:
	Str *str;
};

class Type: public Node{
public:
	Type(string const &name)
	: name{name} {}
	void setRef(){name = "ref " + name;}
	void setNoalias(){name = "noalias " + name;}
	string getName(){return name;}
	virtual void printYaml(YAML::Emitter &out);
private:
	string name;
};

class BinOp: public Exp{
public:
	BinOp(Exp* const exp1, string const &sym, Exp* const exp2)
	: lhs{exp1}, rhs{exp2} {op = whichBinOp(sym);}
	virtual void printYaml(YAML::Emitter &out);
private:
	Exp *lhs, *rhs;
	string op;
	static string whichBinOp(string const &op){
		std::unordered_map<string, string> m = {
			{"*", "mul"}, {"/", "div"}, {"+", "add"},
			{"-", "sub"}, {"<", "lt"}, {">", "gt"},
			{"==", "eq"}, {"!=", "neq"}, {"&&", "and"},
			{"||", "or"}, {"=", "assign"},
		};
		return m[op];
	}
};

class UaryOp: public Exp{
public:
	UaryOp(string const &op, Exp* const exp): op{op}, exp{exp} {}
	virtual void printYaml(YAML::Emitter &out);
private:
	Exp *exp;
	string op;
};

class VarDecl: public Node{
public:
	VarDecl(Type* const type, Var* const var)
	: type{type}, var{var} {}
	Var* getVar(){return var;}
	virtual void printYaml(YAML::Emitter &out);
private:
	Type *type;
	Var *var;
};

class FuncCall: public Exp{
public:
	FuncCall(Globid* const globid, ExpList* const exps = nullptr)
	: globid{globid}, args{exps} {}
	virtual void printYaml(YAML::Emitter &out);
private:
	Globid *globid;
	ExpList *args;
};


class Blk: public Stmt{
public:
	StmtList statements;
	virtual void printYaml(YAML::Emitter &out);
};

class FuncDecl: public Node{
public:
	FuncDecl(Type* const type, Globid* const globid, Blk* const blk)
	: type{type}, globid{globid}, blk{blk} {}
	FuncDecl(Type* const type, Globid* const globid, VarList* const vdecls, Blk* const blk)
	: type{type}, globid{globid}, args{vdecls}, blk{blk} {}
	virtual void printYaml(YAML::Emitter &out);
private:
	Type *type;
	Globid *globid;
	VarList *args = nullptr;
	Blk *blk;
};

class Extern: public Node{
public:
	Extern(Type* const type, Globid* const globid, TypeList* const tdecls = nullptr)
	: type{type}, globid{globid}, args{tdecls} {}
	virtual void printYaml(YAML::Emitter &out);
private:
	Type *type;
	Globid *globid;
	TypeList *args;
};


class Prog: public Node{
public:
	Prog(ExternList* const externs, FuncList* const funcdecls)
	: externs{externs}, funcs{funcdecls} {}
	virtual void printYaml(YAML::Emitter &out);
private:
	ExternList *externs;
	FuncList *funcs;
};


#endif