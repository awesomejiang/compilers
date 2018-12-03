#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <cstdio>

#include <yaml-cpp/yaml.h>

#include <llvm/IR/Value.h>

using std::vector;
using std::string;

class CodeGenContext;

class Exp;
class Stmt;
class VarDecl;
class NType;
class Blk;
class Extern;
class FuncDecl;
class Prog;

using ExpList = vector<Exp*>;
using StmtList = vector<Stmt*>;
using VarList = vector<VarDecl*>;
using TypeList = vector<NType*>;
using ExternList = vector<Extern*>;
using FuncList = vector<FuncDecl*>;


class Node{
public:
	virtual void printYaml(YAML::Emitter &out) {}
	virtual void check() {}
	virtual llvm::Value* codegen(CodeGenContext &context) {return nullptr;}
};


class Stmt: public Node {};

class Exp: public Node{
public:
	Exp(string const &tn = ""): typeName{tn} {}
	virtual void printYaml(YAML::Emitter &out) {}
	string typeName;
};

class Num: public Exp{
public:
	Num(string const &str): val{str} {
		if(str.find(".")!=string::npos)
			typeName = "float";
		else
			typeName = "int";
	}
	virtual void printYaml(YAML::Emitter &out);
	virtual void check();
	virtual llvm::Value* codegen(CodeGenContext &context);
private:
	string val;
};

class Str: public Exp{
public:
	Str(string const &name): name{name.substr(1, name.size()-2)}, Exp{"slit"} {}
	virtual void printYaml(YAML::Emitter &out);
	virtual void check();
	string name;
};

class Ident: public Exp{
public:
	Ident(string const &name): name{name} {}
	virtual void printYaml(YAML::Emitter &out) {}
	virtual llvm::Value* codegen(CodeGenContext &context);

	string name;
};


class Var: public Ident{
public:
	using Ident::Ident;
	virtual void printYaml(YAML::Emitter &out);
	virtual void check();
	virtual llvm::Value* codegen(CodeGenContext &context);
};

class Globid: public Ident{
public:
	using Ident::Ident;
	virtual void printYaml(YAML::Emitter &out);
	virtual void check();
};

class NType: public Node{
public:
	NType(string const &name)
	: name{name} {}
	void setRef(){
		if(name == "void")
			throw std::runtime_error("error: cannot declare ref for void variable.");
		if (isRef){
			throw std::runtime_error("error: cannot declare ref for ref variable.");
		}
		isRef = true;
	}
	void setNoalias(){isNoalias = true;}
	virtual void printYaml(YAML::Emitter &out);
	virtual void check();

	string getName(){return name;}
	bool getRef(){return isRef;}
	bool getNoalias(){return isNoalias;}

private:
	string name;
	bool isRef = false, isNoalias = false;
};


class VarDecl: public Node{
public:
	VarDecl(NType* const _type, Var* const _var)
	: type{_type}, var{_var} {
		if(type->getName() == "void")
			throw std::runtime_error("error: void is not allowed in variable declaration.");
		var->typeName = type->getName(); //note: this command cannot change vars in binop, confused
	}
	NType* getType(){return type;}
	Var* getVar(){return var;}
	virtual void printYaml(YAML::Emitter &out);
	virtual void check();
	virtual llvm::Value* codegen(CodeGenContext &context);
private:
	NType *type;
	Var *var;
};

class FuncDecl: public Node{
public:
	FuncDecl(NType* const type, Globid* const globid, Blk* const blk, VarList* const vdecls = nullptr)
	: type{type}, globid{globid}, args{vdecls}, blk{blk} {
		if(type->getRef())
			throw std::runtime_error("error: function should not return a ref type.");
	}
	virtual void printYaml(YAML::Emitter &out);
	virtual void check();
	virtual llvm::Value* codegen(CodeGenContext &context);
private:
	NType *type;
	Globid *globid;
	VarList *args;
	Blk *blk;
};

class ExpStmt: public Stmt{
public:
	ExpStmt(Exp* const exp): exp{exp} {}
	virtual void printYaml(YAML::Emitter &out);
	virtual void check();
	virtual llvm::Value* codegen(CodeGenContext &context);
private:
	Exp *exp;
};

class AssignStmt: public Stmt{
public:
	AssignStmt(VarDecl* const vdecl, Exp* const exp): vdecl{vdecl}, exp{exp} {}
	virtual void printYaml(YAML::Emitter &out);
	virtual void check();
	virtual llvm::Value* codegen(CodeGenContext &context);
private:
	VarDecl* vdecl;
	Exp *exp;
};

class ReturnStmt: public Stmt{
public:
	ReturnStmt(Exp* const exp = nullptr): exp{exp} {}
	virtual void printYaml(YAML::Emitter &out);
	virtual void check();
	virtual llvm::Value* codegen(CodeGenContext &context);
private:
	Exp *exp;
};

class WhileStmt: public Stmt{
public:
	WhileStmt(Exp* const exp, Stmt* const stmt): exp{exp}, stmt{stmt} {}
	virtual void printYaml(YAML::Emitter &out);
	virtual void check();
	virtual llvm::Value* codegen(CodeGenContext &context);
private:
	Exp *exp;
	Stmt *stmt;
};

class IfStmt: public Stmt{
public:
	IfStmt(Exp* const exp, Stmt* const stmt, Stmt* const elsestmt = nullptr)
	: exp{exp}, stmt{stmt}, elsestmt{elsestmt} {}
	virtual void printYaml(YAML::Emitter &out);
	virtual void check();
	virtual llvm::Value* codegen(CodeGenContext &context);
private:
	Exp *exp;
	Stmt *stmt, *elsestmt;
};

class PrintStmt: public Stmt{
public:
	PrintStmt(Exp* const exp): exp{exp} {}
	virtual void printYaml(YAML::Emitter &out);
	virtual void check();
	virtual llvm::Value* codegen(CodeGenContext &context);
private:
	Exp *exp;
};

class PrintSlitStmt: public Stmt{
public:
	PrintSlitStmt(Str* const str): str{str} {}
	PrintSlitStmt(string const &msg): str{new Str{msg}} {}
	virtual void printYaml(YAML::Emitter &out);
	virtual void check();
	virtual llvm::Value* codegen(CodeGenContext &context);
private:
	Str *str;
};

class BinOp: public Exp{
public:
	BinOp(Exp* const exp1, string const &sym, Exp* const exp2)
	: lhs{exp1}, rhs{exp2} {
		op = whichBinOp(sym);
		setTypeName();
	}
	virtual void printYaml(YAML::Emitter &out);
	virtual void check();
	virtual llvm::Value* codegen(CodeGenContext &context);
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
	void setTypeName(){
		if(op == "eq" || op == "lt" || op == "gt" || op == "and" || op == "or")
			typeName = "int";
		else if(lhs->typeName == rhs->typeName){
			typeName = lhs->typeName;
		}
		else{
			auto name1 = lhs->typeName, name2 = rhs->typeName;
			if(name1 == "cint" || name2 == "cint")
				typeName = "cint";
			if(name1 == "float" || name2 == "float")
				typeName = "float";
			if(name1 == "sfloat" || name2 == "sfloat")
				typeName = "sfloat";
			if(name1 == "int" || name2 == "int")
				typeName = "int";
		}
	}
};

class UaryOp: public Exp{
public:
	UaryOp(string const &op, Exp* const exp): op{op}, exp{exp} {
		if(op == "-")
			typeName = exp->typeName;
		else
			typeName = "int";
	}
	virtual void printYaml(YAML::Emitter &out);
	virtual void check();
	virtual llvm::Value* codegen(CodeGenContext &context);
private:
	Exp *exp;
	string op;
};


class FuncCall: public Exp{
public:
	FuncCall(Globid* const globid, ExpList* const exps = nullptr)
	: globid{globid}, args{exps}, Exp{"function"} {}	//function map is not built when constructor is called 
	virtual void printYaml(YAML::Emitter &out);
	virtual void check();
	virtual llvm::Value* codegen(CodeGenContext &context);
private:
	Globid *globid;
	ExpList *args;
};


class Blk: public Stmt{
public:
	Blk(StmtList const &stmts = {}): statements{stmts} {}

	StmtList statements;
	virtual void printYaml(YAML::Emitter &out);
	virtual void check();
	virtual llvm::Value* codegen(CodeGenContext &context);
};



class Extern: public Node{
public:
	Extern(NType* const type, Globid* const globid, TypeList* const tdecls = nullptr)
	: type{type}, globid{globid}, args{tdecls} {}
	virtual void printYaml(YAML::Emitter &out);
	virtual void check();
	virtual llvm::Value* codegen(CodeGenContext &context);
private:
	NType *type;
	Globid *globid;
	TypeList *args;
};

class Prog: public Node{
public:
	Prog(ExternList* const externs, FuncList* const funcdecls)
	: externs{externs}, funcs{funcdecls} {}
	virtual void printYaml(YAML::Emitter &out);
	virtual void check();
	virtual llvm::Value* codegen(CodeGenContext &context);

private:
	ExternList *externs;
	FuncList *funcs;
};

#endif