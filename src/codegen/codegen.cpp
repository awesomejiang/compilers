#include "node.h"
#include "context.h"

using namespace std;
using namespace llvm;


static Type *typeOf(CodeGenContext &context, NType *type) {
    if(type->getName()=="int" || type->getName()=="cint"){
    	if(type->getRef())
    		return Type::getInt32PtrTy(context.TheContext);
    	else
    		return Type::getInt32Ty(context.TheContext);
    }
    else if(type->getName()=="float" || type->getName()=="sfloat"){
    	if(type->getRef())
    		return Type::getFloatPtrTy(context.TheContext);
    	else
    		return Type::getFloatTy(context.TheContext);
    }
    return Type::getVoidTy(context.TheContext);
}

static Value* LogError(string const &Str)
{
	throw runtime_error(Str);
	return nullptr;
}

Value* Num :: codegen(CodeGenContext &context)
{
	cout << "Generating Num..." << endl;
	if (typeName.find("int") != string::npos)
		return ConstantInt::get(Type::getInt32Ty(context.TheContext), stoi(val), true);
	else if (typeName.find("float") != string::npos)
		return ConstantFP::get(llvm::Type::getFloatTy(context.TheContext), stod(val));
	else
		return LogError("Num: unavailable type name: " + typeName);
}

Value* Ident :: codegen(CodeGenContext &context)
{
	cout << "Generating Ident..." << endl;
	auto locals = context.getLocals();
	if (locals.find(name) == locals.end())
		return LogError("Ident: undeclared identifier: " + name);
	return context.Builder.CreateLoad(locals[name], "");
}

Value* Var :: codegen(CodeGenContext &context)
{
	cout << "Generating Var..." << endl;
	auto locals = context.getLocals();
	if (locals.find(name) == locals.end())
		return LogError("Var: undeclared identifier: " + name);
	return context.Builder.CreateLoad(locals[name], "");
}

Value* ExpStmt :: codegen(CodeGenContext &context)
{
	cout << "Generating ExpStmt..." << endl;
	if (!exp) return LogError("ExpStmt: empty exp");
	return exp->codegen(context);
}

Value* ReturnStmt :: codegen(CodeGenContext& context)
{
	cout << "Generating ReturnStmt..." << endl;
    if (!exp) return LogError("ReturnStmt: empty exp");
    Value* returnValue = exp->codegen(context);
    context.setCurrentReturnValue(returnValue);
    return returnValue;
}

Value* FuncCall :: codegen(CodeGenContext &context)
{
	cout << "Generating FuncCall..." << endl;
	Function* function = context.module->getFunction(globid->name.c_str());
	if (!function)
		return LogError("FuncCall: empty function");
	if (function->arg_size() != args->size())
		return LogError("FuncCall: incorrect number of arguments");
	vector<Value*> arguments;
	if(args)
		for(auto arg: *args)
			arguments.push_back(arg->codegen(context));

	CallInst* call = context.Builder.CreateCall(function, makeArrayRef(arguments), "calltmp");
	return call;
}

Value* Blk :: codegen(CodeGenContext &context)
{
	cout << "Generating Blk..." << endl;
	Value* last = nullptr;
	for(auto stmt: statements){
		last = stmt->codegen(context);
	}
	return last;
}

Value* FuncDecl :: codegen(CodeGenContext &context)
{
	cout << "Generating FuncDecl..." << endl;
	// prototype double(double, double, ...)
	vector<llvm::Type*> argTypes;
	VarList::const_iterator it;
	if (args)
		for (it = args->begin(); it != args->end(); it++)
			argTypes.push_back(typeOf(context, (*it)->getType()));

	FunctionType* funcType = FunctionType::get(typeOf(context, type), makeArrayRef(argTypes), false);
	Function* function = Function::Create(funcType, GlobalValue::InternalLinkage, globid->name.c_str(), context.module.get());

	BasicBlock* bblock = BasicBlock::Create(context.TheContext, "entry", function, 0);
	context.pushBlock(bblock);
	context.Builder.SetInsertPoint(bblock);

	auto locals = context.getLocals();
	auto localTypes = context.getTypes();

	Function::arg_iterator ait = function->arg_begin();
	Value* argValue;
	if (args)
		for (it = args->begin(); it != args->end(); it++) {
			auto varName = (*it)->getVar()->name;
			(*it)->codegen(context);
			argValue = &*ait++;
			argValue->setName(varName.c_str());
			// alloc an Value for var and assign to it 
			locals[varName] = context.Builder.CreateAlloca(typeOf(context, (*it)->getType()), 0, varName.c_str());
			StoreInst* inst = context.Builder.CreateStore(argValue, locals[varName]);
			//also add types along with locals
			localTypes[varName] = (*it)->getType();
		}

	blk->codegen(context);
	context.Builder.CreateRet(context.getCurrentReturnValue());

	context.popBlock();
	return function;
}

Value* AssignStatementUtil(CodeGenContext &context, Exp* hs, Exp* rhs)
{
	cout << "Generating AssignStatementUtil..." << endl;
	Var* lhs = dynamic_cast<Var*>(hs);
	auto locals = context.getLocals();
	if (locals.find(lhs->name) == locals.end())
		return nullptr;

	Value* assign_gen = rhs->codegen(context);
	Value* assign_rhs;

	if (lhs->typeName.find("float") != string::npos) {
		if (rhs->typeName.find("float") != string::npos)
			assign_rhs = assign_gen;
		else
			assign_rhs = context.Builder.CreateSIToFP(assign_gen, Type::getFloatTy(context.TheContext), "");
	} else if (lhs->typeName.find("int") != string::npos) {
		if (rhs->typeName.find("int") != string::npos)
			assign_rhs = assign_gen;
		else
			assign_rhs = context.Builder.CreateFPToSI(assign_gen, Type::getInt32Ty(context.TheContext), "");
	} else {
		assign_rhs = assign_gen;
	}

	Value* clhs = assign_rhs;
	auto type = context.getTypes()[lhs->name];
	if (type->getRef()) {
		clhs = context.Builder.CreateLoad(locals[lhs->name], "");
		return context.Builder.CreateStore(assign_rhs, clhs);
	} else {
		return context.Builder.CreateStore(clhs, locals[lhs->name]);
	}

	return nullptr;
}

Value* AssignStmt :: codegen(CodeGenContext &context)
{
	cout << "Generating AssignStmt..." << endl;
	Value* alloc = vdecl->codegen(context);
	if (exp) 
		AssignStatementUtil(context, vdecl->getVar(), exp); // NAssignment_codeGen(vdecl->var, exp)

	return alloc;
}

Value* Extern :: codegen(CodeGenContext &context)
{
	cout << "Generating Extern..." << endl;
	vector<Type*> types;
	if(args)
		for (auto it = args->begin(); it != args->end(); it++)
			types.push_back(typeOf(context, *it));
	FunctionType* funcType = FunctionType::get(typeOf(context, type), makeArrayRef(types), false);
	Function* function = Function::Create(funcType, GlobalValue::ExternalLinkage, globid->name.c_str(), context.module.get());
	return function;
}


Value* VarDecl :: codegen(CodeGenContext &context)
{
	cout << "Generating VarDecl..." << endl;
	AllocaInst* alloc = context.Builder.CreateAlloca(typeOf(context, type), 0, var->name.c_str());
	context.getLocals()[var->name] = alloc;
	context.getTypes()[var->name] = type;
	cout << "VarDecl: " << var->name << endl;
	return alloc;
}

Value* PrintStmt :: codegen(CodeGenContext &context)
{
	cout << "Generating PrintStmt..." << endl;
	Function* function = context.module.get()->getFunction("printf");
	Value* vstr;
	if (exp->typeName.find("int") != string::npos)
		vstr = context.Builder.CreateGlobalStringPtr("%d");
	else
		vstr = context.Builder.CreateGlobalStringPtr("%f");
	vector<Value*> int32_params{vstr, exp->codegen(context)};
	CallInst* call = context.Builder.CreateCall(function, int32_params, "");
	return call;
}

Value* PrintSlitStmt :: codegen(CodeGenContext &context)
{
	cout << "Generating PrintSlitStmt..." << endl;
	Function* function = context.module.get()->getFunction("printf");
	string temp = str->name.substr(1, str->name.size() - 2);
	Value *vstr = context.Builder.CreateGlobalStringPtr(temp);
	vector<Value*> int32_params{vstr};
	CallInst* call = context.Builder.CreateCall(function, int32_params, "");
	return call;
}

Value* IfStmt :: codegen(CodeGenContext &context)
{
	cout << "Generating IfStmt..." << endl;
	Value* condV = exp->codegen(context);
	if (!condV) return LogError("IfStmt: empty exp");

	condV = context.Builder.CreateICmpNE(condV, context.Builder.getInt1(0), "");
	Function* function = context.getCurrentBlock()->getParent();

	BasicBlock* bbThen = BasicBlock::Create(context.TheContext, "then", function);
	BasicBlock* bbElse = BasicBlock::Create(context.TheContext, "else");
	BasicBlock* bbMerg = BasicBlock::Create(context.TheContext, "ifcon");

	context.Builder.CreateCondBr(condV, bbThen, bbElse);
	context.Builder.SetInsertPoint(bbThen);

	Value* thenV = stmt->codegen(context);

	context.Builder.CreateBr(bbMerg);
	bbThen = context.Builder.GetInsertBlock();

	function->getBasicBlockList().push_back(bbElse);
	context.Builder.SetInsertPoint(bbElse);

	if (elsestmt)
		elsestmt->codegen(context);

	context.Builder.CreateBr(bbMerg);
	bbElse = context.Builder.GetInsertBlock();

	function->getBasicBlockList().push_back(bbMerg);
	context.Builder.SetInsertPoint(bbMerg);

	return nullptr;
}

Value* BinOp :: codegen(CodeGenContext &context)
{
	cout << "Generating BinOp..." << endl;
	Instruction::BinaryOps instBin;
	Instruction::OtherOps instOther;
	CmpInst::Predicate pred;

	Value* clhs = lhs->codegen(context);
	Value* crhs = rhs->codegen(context);
	if (!clhs || !crhs) return nullptr;

	if (op == "&&") {
		instBin = Instruction::And;
		return context.Builder.CreateAnd(clhs, crhs, "");
	} else if (op == "||") {
		instBin = Instruction::Or;
		return context.Builder.CreateOr(clhs, crhs, "");
	} else if (op == "=") {
		return AssignStatementUtil(context, lhs, rhs);
	}

	// reference type
	auto types = context.getTypes();
	for(auto p: {make_pair(lhs, clhs), make_pair(rhs, crhs)}){
		auto ptr = dynamic_cast<Var*>(p.first);
		if(ptr && types[ptr->name]->getRef())
			p.second = context.Builder.CreateLoad(p.second, "");
	}

	// tpye conversion
	Value* llhs, * rrhs;
	cout << typeName << "\t" << lhs->typeName << "\t" << rhs->typeName << endl;
	if (typeName == "float") {
		if (lhs->typeName == "float")
			llhs = clhs;
		else
			llhs = context.Builder.CreateSIToFP(clhs, Type::getFloatTy(context.TheContext), "");
		if (rhs->typeName == "float")
			rrhs = crhs;
		else
			rrhs = context.Builder.CreateSIToFP(crhs, Type::getFloatTy(context.TheContext), "");
	} else if (typeName == "int") {
		if (lhs->typeName == "int")
			llhs = clhs;
		else
			llhs = context.Builder.CreateFPToSI(clhs, Type::getInt32Ty(context.TheContext), "");
		if (rhs->typeName == "int")
			rrhs = crhs;
		else
			rrhs = context.Builder.CreateFPToSI(crhs, Type::getInt32Ty(context.TheContext), "");
	} else {
		llhs = clhs;
		rrhs = crhs;
	}

	Value* value = nullptr;
	cout << "Ret bin: " << typeName << endl;
	if (typeName == "float" || typeName == "sfloat") {
		if (op == "+") value = context.Builder.CreateFAdd(llhs, rrhs, "");
		else if (op == "-") value = context.Builder.CreateFSub(llhs, rrhs, "");
		else if (op == "*") value = context.Builder.CreateFMul(llhs, rrhs, "");
		else if (op == "/") value = context.Builder.CreateFDiv(llhs, rrhs, "");
		else if (op == "==") value= context.Builder.CreateFCmpOEQ(llhs, rrhs, "");
		else if (op == "<") value = context.Builder.CreateFCmpOLT(llhs, rrhs, "");
		else if (op == ">") value = context.Builder.CreateFCmpOGT(llhs, rrhs, "");
		if (typeName == "float") {
			FastMathFlags fmf;
			fmf.setUnsafeAlgebra();
			((BinaryOperator*) value)->setFastMathFlags(fmf);
		}
		return value;
	} else if (typeName == "int" || typeName == "cint") {
		if (op == "==") value = context.Builder.CreateICmpEQ(llhs, rrhs, "");
		else if (op == "<") value = context.Builder.CreateICmpSLT(llhs, rrhs, "");
		else if (op == ">") value = context.Builder.CreateICmpSGT(llhs, rrhs, "");
		else {
			if (typeName == "int") {
				if (op == "+") value = context.Builder.CreateAdd(llhs, rrhs, "");
				else if (op == "-") value = context.Builder.CreateSub(llhs, rrhs, "");
				else if (op == "*") value = context.Builder.CreateMul(llhs, rrhs, "");
				else if (op == "/") value = context.Builder.CreateSDiv(llhs, rrhs, "");
			} else { // cint
				if (op == "/")
					value = context.Builder.CreateSDiv(llhs, rrhs, "");
				else {
					Value* errV;
					string errInfo;
					if (op == "+") {
						errV = Intrinsic::getDeclaration(
							context.module.get(), Intrinsic::sadd_with_overflow, Type::getInt32Ty(context.TheContext));
						errInfo = ",Add operation overflows!,";
					} else if (op == "-") {
						errV = Intrinsic::getDeclaration(
							context.module.get(), Intrinsic::ssub_with_overflow, Type::getInt32Ty(context.TheContext));
						errInfo = ",Sub operation overflows!,";
					} else if (op == "*") {
						errV = Intrinsic::getDeclaration(
							context.module.get(), Intrinsic::smul_with_overflow, Type::getInt32Ty(context.TheContext));
						errInfo = ",Mul operation overflows!,";
					}
					auto *SBinaryOperatorWithOverflow = context.Builder.CreateCall(errV, {llhs, rrhs}, "t");
					auto *SBinaryOperator = context.Builder.CreateExtractValue(SBinaryOperatorWithOverflow, 0, "binaryop");
					auto *Overflow = context.Builder.CreateExtractValue(SBinaryOperatorWithOverflow, 1, "obit");

					Function* function = context.getCurrentBlock()->getParent();
					BasicBlock* bbNormal = BasicBlock::Create(context.TheContext, "normal", function);
					BasicBlock* bbOverflow = BasicBlock::Create(context.TheContext, "overflow", function);
					BasicBlock* bbMerg = BasicBlock::Create(context.TheContext, "continue");

					context.Builder.CreateCondBr(Overflow, bbNormal, bbOverflow);
					context.Builder.SetInsertPoint(bbNormal);
					context.Builder.CreateBr(bbMerg);

					function->getBasicBlockList().push_back(bbOverflow);
					context.Builder.SetInsertPoint(bbOverflow);
					auto printError = make_shared<PrintSlitStmt>(errInfo);
					printError->codegen(context);
					context.Builder.CreateBr(bbMerg);

					function->getBasicBlockList().push_back(bbMerg);
					context.Builder.SetInsertPoint(bbMerg);
					return SBinaryOperator;
				}
			}
		}
	}
	return value;
}

Value* UaryOp :: codegen(CodeGenContext &context)
{
	cout << "Generating UaryOp..." << endl;
	shared_ptr<Num> zero = make_shared<Num>("0");
	shared_ptr<BinOp> bin = make_shared<BinOp>(zero.get(), op, exp);
	if (op.find("-") != string::npos)
		return bin->codegen(context);
	else if (op.find("!") != string::npos)
		return context.Builder.CreateNot(exp->codegen(context), "");
	return LogError("Invalid UaryOp.");
}

Value* WhileStmt :: codegen(CodeGenContext &context)
{
	cout << "Generating WhileStmt..." << endl;
	Function* function = context.getCurrentBlock()->getParent();

	BasicBlock* bbCond = BasicBlock::Create(context.TheContext, "whilecond", function);
	BasicBlock* bbThen = BasicBlock::Create(context.TheContext, "loop");
	BasicBlock* bbMerg = BasicBlock::Create(context.TheContext, "whilecon");

	context.Builder.CreateBr(bbCond);
	context.Builder.SetInsertPoint(bbCond);
	Value* condV = exp->codegen(context);
	context.Builder.CreateCondBr(condV, bbThen, bbMerg);

	function->getBasicBlockList().push_back(bbThen);
	context.Builder.SetInsertPoint(bbThen);
	Value* thenV = stmt->codegen(context);

	context.Builder.CreateBr(bbCond);
	bbThen = context.Builder.GetInsertBlock();

	function->getBasicBlockList().push_back(bbMerg);
	context.Builder.SetInsertPoint(bbMerg);
	return nullptr;
}

Value* Prog :: codegen(CodeGenContext &context)
{
	cout << "Generating Prog..." << endl;
	if (externs)
		for(auto ext: *externs)
			ext->codegen(context);
	if(funcs)
		for(auto func: *funcs)
			func->codegen(context);
	return nullptr;
}
