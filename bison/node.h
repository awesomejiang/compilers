#include <iostream>
#include <vector>

using StmtList = std::vector<Stmt*>;
using ExpList = std::vector<Exp*>;
using VarList = std::vector<VarDecl*>


class Node;

class Exp: public Node{

};

class Stmt: public Node{

};

class Integer: public Exp{
public:
	Integer(long long val): val{val} {}
private:
long long val;
};

class Float: public Exp{
public:
	Float(double val): val{val} {}
private:
	double val;
};

class Ident: public Exp{
public:
	Ident(std::string const &name): name{name} {}
private:
	std::string name;
};

class MethodCall: public Exp{
public:
	MethodCall(Ident const &id, ExpList const &args)
	: id{id}, args{args} {}
	MethodCall(Ident const &id): id{id} {}
};

class BinaryOperator: public Exp{
public:
	BinaryOperator(Exp const &lhs, int op, Exp const &rhs)
	: lhs{lhs}, rhs{rhs}, op{op};
private:
	int op;
	Exp lhs, rhs;
};

class Assignment: public Exp{
public:
	Assignment(Ident const &lhs, Ident const &rhs):
	: lhs{lhs}, rhs{rhs} {}
private:
	Exp lhs, rhs;
};

class Blk: public Exp



















