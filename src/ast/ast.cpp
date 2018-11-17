#include "node.h"

using namespace YAML;

void ExpStmt::printYaml(Emitter &out){
	out << Key << "name" << Value << "expstmt";

	out << Key << "exp" << Value;
	out << BeginMap;
	exp->printYaml(out);
	out << EndMap;
}

void AssignStmt::printYaml(Emitter &out){
	out << Key << "name" << Value << "vardeclstmt";

	out << Key << "vdecl" << Value;
	out << BeginMap;
	vdecl->printYaml(out);
	out << EndMap;

	out << Key << "exp" << Value;
	out << BeginMap;
	exp->printYaml(out);
	out << EndMap;
}

void ReturnStmt::printYaml(Emitter &out){
	out << Key << "name" << Value << "ret";

	if(exp){
		out << Key << "exp" << Value;
		out << BeginMap;
		exp->printYaml(out);
		out << EndMap;
	}
}

void WhileStmt::printYaml(Emitter &out){
	out << Key << "name" << Value << "while";

	out << Key << "cond" << Value;
	out << BeginMap;
	exp->printYaml(out);
	out << EndMap;

	out << Key << "stmt" << Value;
	out << BeginMap;
	stmt->printYaml(out);
	out << EndMap;
}

void IfStmt::printYaml(Emitter &out){
	out << Key << "name" << Value << "if";

	out << Key << "cond" << Value;
	out << BeginMap;
	exp->printYaml(out);
	out << EndMap;

	out << Key << "stmt" << Value;
	out << BeginMap;
	stmt->printYaml(out);
	out << EndMap;

	if(elsestmt){
		out << Key << "else_stmt" << Value;
		out << BeginMap;
		elsestmt->printYaml(out);
		out << EndMap;
	}
}

void PrintStmt::printYaml(Emitter &out){
	out << Key << "name" << Value << "print";

	out << Key << "exp" << Value;
	out << BeginMap;
	exp->printYaml(out);
	out << EndMap;
}

void PrintSlitStmt::printYaml(Emitter &out){
	out << Key << "name" << Value << "printslit";
	out << Key << "string" << Value << str->name;
}

void NType::printYaml(Emitter &out){
	out << Key << "type" << Value << string(isNoalias?"noalias ":"") + string(isRef?"ref ":"") + name;
}

void BinOp::printYaml(Emitter &out){
	if(op == "assign"){
		out << Key << "name" << Value << "assign";

		out << Key << "var" << Value << static_cast<Var*>(lhs)->name;	//lhs must be a var

		out << Key << "exp" << Value;
		out << BeginMap;
		rhs->printYaml(out);
		out << EndMap;	
	}
	else{
		out << Key << "name" << Value << "binop";

		out << Key << "op" << Value << op;

		out << Key << "lhs" << Value;
		out << BeginMap;
		lhs->printYaml(out);
		out << EndMap;

		out << Key << "rhs" << Value;
		out << BeginMap;
		rhs->printYaml(out);
		out << EndMap;
	}
}

void UaryOp::printYaml(Emitter &out){
	out << Key << "name" << Value << "uop";

	out << Key << "op" << Value << (op=="!"? "not": "minus");

	out << Key << "exp" << Value;
	out << BeginMap;
	exp->printYaml(out);
	out << EndMap;
}

void Num::printYaml(Emitter &out){
	if(typeName == "float")
		out << Key << "name" << Value << "flit";
	else
		out << Key << "name" << Value << "lit";
	out << Key << "value" << Value << val;
}

void Str::printYaml(Emitter &out){
	out << Key << "name" << Value << "slit";
	out << Key << "string" << Value << name;
}

void Var::printYaml(Emitter &out){
	out << Key << "name" << Value << "varval";
	out << Key << "var" << Value << name;
}

void Globid::printYaml(Emitter &out){
	out << Key << "globid" << Value << name;
}

void VarDecl::printYaml(Emitter &out){
	out << Key << "node" << Value << "vdecl";
	type->printYaml(out);
	out << Key << "var" << Value << var->name;
}

void FuncCall::printYaml(Emitter &out){
	out << Key << "name" << Value << "funccall";
	globid->printYaml(out);
	out << Key << "params" << Value;
	out << BeginMap;
	out << Key << "name" << Value << "exps";

	if(args){
		out << Key << "exps" << Value;
		out << BeginSeq;
		for(auto arg: *args){
			out << BeginMap;
			out << Newline;
			arg->printYaml(out);
			out << EndMap;
		}
		out << EndSeq;
	}

	out << EndMap;
}

void Blk::printYaml(Emitter &out){
	out << Key << "name" << Value << "blk";
	out << Key << "contents" << Value;

	out << BeginMap;
	out << Key << "name" << Value << "stmts";
	out << Key << "stmts" <<Value;

	out << BeginSeq;
	for(auto stmt: statements){
		out << BeginMap;
		out << Newline;
		stmt->printYaml(out);
		out << EndMap;
	}
	
	out << EndSeq;

	out << EndMap;
}

void FuncDecl::printYaml(Emitter &out){
	out << BeginMap;
	out << Newline;

	out << Key << "name" << Value << "func";

	out << Key << "ret_type" << Value << type->getName();

	globid->printYaml(out);

	out << Key << "blk" << Value;
	out << BeginMap;
	blk->printYaml(out);
	out <<EndMap;

	if(args){
		out << Key << "vdecls" << Value;
		out << BeginMap;
		out << Key << "name" << Value << "vdecls";
		out << Key << "vars" << Value;
		out << BeginSeq;
		for(auto arg: *args){
			out << BeginMap;
			out <<Newline;
			arg->printYaml(out);
			out << EndMap;
		}
		out << EndSeq;
		out << EndMap;
	}

	out << EndMap;
}

void Extern::printYaml(Emitter &out){
	out << BeginMap;
	out << Newline;

	out << Key << "name" << Value << "extern";
	out << Key << "ret_type" << Value << type->getName();
	globid->printYaml(out);

	if(args){
		out << Key << "tdecls" << Value;
		out << BeginMap;
		out << Key << "name" << Value << "tdecls";
		out << Key << "types" << Value;
		out << BeginSeq;
		for(auto type: *args){
			out << BeginMap;
			out << type->getName();
			out << EndMap;
		}
		out << EndSeq;
		out << EndMap;
	}

	out << EndMap;
}

void Prog::printYaml(Emitter &out){
	out << BeginMap;
	out << Key << "name" << Value << "prog";

	//functions
	out << Key << "funcs" << Value;
	out << BeginMap;
	out << Key << "name" << Value << "funcs";
	out << Key << "funcs" << Value;
	out << BeginSeq;
	for(auto func: *funcs){
		func->printYaml(out);
	}
	out << EndSeq;
	out << EndMap;

	//externs
	out << Key << "externs" << Value;
	out << BeginMap;
	out << Key << "name" << Value << "externs";
	out << Key << "externs" << Value;
	out << BeginSeq;
	for(auto ext: *externs){
		ext->printYaml(out);
	}
	out << EndSeq;
	out << EndMap;
	out << EndMap;
}
