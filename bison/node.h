#include <iostream>
#include <vector>

class Node{};

class Stmt: public Node{
public:
	Stmt()
};

class Exp: public Node{

};

class Type: public Node{
public:
	Type(std::string const &name, bool const &ref = false)
	: name{name}, noalias{ref} {}
private:
	std::string name;
	bool noalias;
};

class BinOp: public Exp{
public:
	BinOp(Exp const *exp1, std::string const &op. Exp const *exp2)
	: lhs{*exp1}, rhs{*exp2}, op{op} {}
private:
	Exp lhs, rhs;
	std::string op;
};

class UaryOp: public Exp{
public:
	UaryOp(std::string const &op, Exp const *exp): op{op}, exp{*exp} {}
private:
	Exp exp;
	std::string op;
};

class Num: public Exp{
public:
	Num(double val): val{val} {}
private:
	double val;
};

class Str: public Exp{
public:
	Str(std::string const &str): str{str} {}
private:
	std::String str;
}

class Ident: public Exp{
public:
	Ident(std::string const &name): name{name} {}
private:
	std::string name;
};

class Var: public Ident{};

class Globid: public ident{};

class VarDecl: public Node{
public:
	VarDecl(Type const *type, Var const *var)
	: type{*type}, name{*var} {}
private:
	Type type;
	Var var;
};

class FuncCall: public Exp{
public:
	FuncCall(Globid const *globid, std::vector<Exp*> const *exps)
	: globid{*globid}, args{*exps} {}
private:
	Globid globid;
	std::vector<Exp*> args;
};


class Blk: public Node{
private:
	std::vector<Stmt*> statements;
};

class FuncDecl: public Node{
public:
	FuncDecl(Type const *type, Globid CONST *globid, std::vector<VarDecl*> const *vdecls, Blk const *blk)
	: type{*type}, globid{*globid}, args{*args}, blk{*blk} {}
	FuncDecl(Type const *type, Globid CONST *globid, Blk const *blk)
	: type{*type}, globid{*globid}, blk{*blk} {}
private:
	Type type;
	Globid globid;
	std::vector<VarDecl*> args;
	Blk blk;
};

class Extern: public Node{
public:
	Extern(Type const *type, Globid const *globid, std::vector<Type*> const *tdecls)
	: type{*type}, globid{*globid}, args{*tdecls} {}
	Extern(Type const *type, Globid const *globid)
	: type{*type}, globid{*globid} {}
	Type type;
	Globid globid;
	std::vector<Type*> args;
};


class Prog: public Node{
public:
	Prog(std::vector<Extern*> const *externs, std::vector<Func*> const *func)
	: externs{*externs}, func{*func} {}
private:
	std::vector<Extern*> externs;
	std::vector<Func*> funcs;
};
















