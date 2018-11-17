#include "node.h"

using namespace std;
using namespace llvm;

static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
static std::unique_ptr<Module> TheModule;
static std::map<std::string, Value *> NamedValues;

static Type *typeOf(NType * const type) {
    if(type->getName()=="int" || type->getName()=="cint"){
    	if(type->getRef())
    		return Type::getInt32PtrTy(TheContext);
    	else
    		return Type::getInt32Ty(TheContext);
    }
    else if(type->getName()=="float" || type->getName()=="sfloat"){
    	if(type->getRef())
    		return Type::getFloatPtrTy(TheContext);
    	else
    		return Type::getFloatTy(TheContext);
    }
    return Type::getVoidTy(TheContext);
}

static Value* LogError(string const &Str)
{
	printf("%s\n", Str.c_str());
	return nullptr;
}

Value* Num :: codegen()
{
	if (typeName == "int")
		return ConstantInt::get(Type::getInt32Ty(TheContext), stoi(val), true);
	else if (typeName == "float")
		return ConstantFP::get(llvm::Type::getFloatTy(TheContext), stod(val));
	else
		return LogError("Num: unavailable type name: " + typeName);
}

Value* Ident :: codegen()
{
	if (NamedValues.find(name) == NamedValues.end())
		return LogError("Ident: undeclared identifier: " + name);
	return Builder.CreateLoad(NamedValues[name], "");
}

Value* Var :: codegen()
{
	if (NamedValues.find(name) == NamedValues.end())
		return LogError("Var: undeclared identifier: " + name);
	return Builder.CreateLoad(NamedValues[name], "");
}

Value* BinOp :: codegen()
{
	Value* L = lhs->codegen();
	Value* R = rhs->codegen();
	if (!L || !R) return nullptr;

	switch (op[0])
	{
	case '+':
		return Builder.CreateFAdd(L, R, "addtmp");
	case '-':
		return Builder.CreateFSub(L, R, "subtmp");
	case '*':
		return Builder.CreateFMul(L, R, "multmp");
	case '<':
		L = Builder.CreateFCmpULT(L, R, "cmptmp");
		return Builder.CreateUIToFP(L, llvm::Type::getDoubleTy(TheContext), "booltmp");
	default:
		return LogError("BinOp: invalid binary operation: " + string(op));
	}
}

Value* ExpStmt :: codegen()
{
	if (!exp) return LogError("ExpStmt: empty exp");
	return exp->codegen();
}

Value* ReturnStmt :: codegen()
{
	if (!exp) return LogError("ReturnStmt: empty exp");
	return exp->codegen();
}

Value* FuncCall :: codegen()
{
	Function* function = TheModule.get()->getFunction(globid->name.c_str());
	if (!function)
		return LogError("FuncCall: empty function");
	if (function->arg_size() != args->size())
		return LogError("FuncCall: incorrect number of arguments");
	vector<Value*> arguments;
	for (unsigned i = 0, e = args->size(); i != e; i++) {
		arguments.push_back((*args)[i]->codegen());
		if (!arguments.back())
			return nullptr;
	}
	CallInst* call = Builder.CreateCall(function, makeArrayRef(arguments), "calltmp");
	return call;
}

Value* Blk :: codegen()
{
	Value* last = statements.back()->codegen();
	return last;
}

Value* FuncDecl :: codegen()
{
	// prototype double(double, double, ...)
	vector<llvm::Type*> types;
	VarList::const_iterator it;
	if (args)
		for (it = args->begin(); it != args->end(); it++)
			types.push_back(typeOf((*it)->getType()));
	FunctionType* funcType = FunctionType::get(typeOf(type), makeArrayRef(types), false);
	Function* function = Function::Create(funcType, GlobalValue::InternalLinkage, globid->name.c_str(), TheModule.get());

	BasicBlock* bblock = BasicBlock::Create(TheContext, "entry", function, 0);
	Builder.SetInsertPoint(bblock);

	NamedValues.clear();
	for (auto &arg : function->args())
		NamedValues[arg.getName()] = &arg;

	//Function::arg_iterator ait = function->arg_begin();
	//Value* argValue;
	//if (args)
	//	for (it = *args.begin(); it != *args.end(); it++) {
	//		(**it).codegen();
	//		argValue = &*ait++;
	//		argValue->setName((*it)->var->name->name.c_str());
	//		StoreInst* inst = Builder.CreateStore(argValue, TheContext[(*it)->var->name->name]);
	//	}

	//if (Value* retVal = args)
	//Builder.CreateRet(TheContext)
	return nullptr;
}

Value* AssignStmt::codegen(){}
Value* PrintSlitStmt::codegen(){}
Value* Str::codegen(){}
Value* Node::codegen(){}
Value* Prog::codegen(){}
Value* NType::codegen(){}
Value* Extern::codegen(){}
Value* Globid::codegen(){}
Value* IfStmt::codegen(){}
Value* UaryOp::codegen(){}
Value* VarDecl::codegen(){}
Value* PrintStmt::codegen(){}
Value* WhileStmt::codegen(){}
Value* Exp::codegen(){}








