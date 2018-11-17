#include "node.h"

static std::unordered_map<string, vector<NType>> functionMap;
static std::stack<std::unordered_set<string>> varTable;


void ExpStmt::check(){
	exp->check();
}

void AssignStmt::check(){
	//vardecl is for ref variable and rhs is not a variable
	if(vdecl->getType()->getRef() && exp->typeName != "")
		throw std::runtime_error("error: ref variable can only be initialized by a variable.");
	//update varTable
	if(varTable.empty())
		varTable.emplace();
	varTable.top().insert(vdecl->getVar()->name);

	exp->check();
}

void ReturnStmt::check(){
	if(exp)
		exp->check();
}

void WhileStmt::check(){
	exp->check();
	stmt->check();
}

void IfStmt::check(){
	exp->check();
	stmt->check();
	if(elsestmt)
		elsestmt->check();
	
}

void PrintStmt::check(){
	exp->check();
}

void PrintSlitStmt::check(){}

void NType::check(){}

void BinOp::check(){
	if(op == "assign")
		rhs->check();
	else{
		lhs->check();
		rhs->check();
	}
}

void UaryOp::check(){
	exp->check();
}

void Num::check(){}

void Str::check(){}

void Var::check(){
	auto currScope = varTable.top();
	if(currScope.find(name)==currScope.end())
		throw std::runtime_error("error: undefined variable.");
}

void Globid::check(){}

void VarDecl::check(){}

void FuncCall::check(){
	//check if globid is stored in functionMap
	bool valid = true;
	if(functionMap.find(globid->name)!=functionMap.end()){
		auto types = functionMap[globid->name];
		if(!args){ //args is empty
			if(types.size()!=1)
				valid = false;
		}
		else{
			 if((*args).size()+1==types.size()){
			 	for(auto i=0; i<types.size()-1; ++i){
			 		//if the type fits. Note: both cint and int fits int
			 		if(types[i].getName().find((*args)[i]->getTypeName())==string::npos){
			 			valid = false;
			 			break;
			 		}
			 		//if type is ref, arg must be var
			 		if(types[i].getRef() && (*args)[i]->typeName != ""){
			 			throw std::runtime_error("error: passed argument for ref function argument must be a variable.");
			 		}
			 	}
			 }
			 else 
			 	valid = false;
		}
		typeName = types.back().getName();
	}
	else
		valid = false;

	if(!valid)
		throw std::runtime_error("error: undefined function.");
}

void Blk::check(){
	std::unordered_set<string> newScope;
	if(!varTable.empty()){
		newScope = varTable.top();
	}
	varTable.push(newScope);
	
	for(auto stmt: statements)
		stmt->check();

	varTable.pop();
}

void FuncDecl::check(){
	//record function info into functionMap and vartable
	varTable.emplace();
	vector<NType> types;
	if(args){
		for(auto vdeclPtr: *args){
			types.push_back(*(vdeclPtr->getType()));
			varTable.top().insert(vdeclPtr->getVar()->name);
		}
	}
	types.push_back(*type);	//add return type at tail
	functionMap[globid->name] = types;

	blk->check();
	varTable.pop();
}

void Extern::check(){
	//record function info into functionMap
	vector<NType> types;
	if(args){
		for(auto typePtr: *args)
			types.push_back(*typePtr);
	}
	types.push_back(*type);	//add return type at tail
	functionMap[globid->name] = types;
}

void Prog::check(){
	for(auto ext: *externs)
		ext->check();
	for(auto func: *funcs)
		func->check();

	//check if there is a "run" function
	if(functionMap.find("run")==functionMap.end()	//no run
	 || functionMap["run"].size()!=1	//	has args
	 || functionMap["run"].back().getName()!="int")	// do not return int
		throw	std::runtime_error("error: there is no proper 'run' function.");
}
