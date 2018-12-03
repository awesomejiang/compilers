#include "node.h"
#include "context.h"

using namespace std;
using namespace llvm;

/* *** Util functions ***/ 
static Type *typeOf(CodeGenContext &context, NType *type) {
    if(type->getName().find("int") != string::npos){
    	if(type->getRef())
    		return Type::getInt32PtrTy(context.TheContext);
    	else
    		return Type::getInt32Ty(context.TheContext);
    }
    else if(type->getName().find("float") != string::npos){
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

static Value *removeRef(CodeGenContext &context, Exp *exp, Value *value){
	if(!dynamic_cast<Var*>(exp))
		return value;

	auto name = dynamic_cast<Var*>(exp)->name;
	auto &types = context.getTypes();
	if(types.find(name)!=types.end() && types[name]->getRef())
		value = context.Builder.CreateLoad(value);

	return value;
}

static void createPrintf(CodeGenContext& context) {
    vector<Type*> printfArgTypes{Type::getInt8PtrTy(context.TheContext)}; //char*

    FunctionType* retType = FunctionType::get(
    	Type::getInt32Ty(context.TheContext),
    	makeArrayRef(printfArgTypes),
    	true
    );

    Function *func = Function::Create(
                retType, Function::ExternalLinkage,
                Twine("printf"),
                context.module.get()
           );
    func->setCallingConv(CallingConv::C);
}

static Value *callGetarg(CodeGenContext& context, ExpList *args) {
	Value * num = args->front()->codegen(context);
	int index = dyn_cast<ConstantInt>(num)->getSExtValue();
	auto inputArgs = context.getArgs();
	if(inputArgs.size() <= index)
		return LogError("error: wrong number of args is passed.");
	auto newNum = make_shared<Num>(inputArgs[index]);
	return newNum->codegen(context);
}

/* *** Start implementing codegen functions here *** */
Value* Num :: codegen(CodeGenContext &context)
{
	//cout << "Generating Num..." << endl;
	if (typeName.find("int") != string::npos)
		return ConstantInt::get(Type::getInt32Ty(context.TheContext), stoi(val), true);
	else if (typeName.find("float") != string::npos)
		return ConstantFP::get(Type::getFloatTy(context.TheContext), stod(val));
	else
		return LogError("Num: unavailable type name: " + typeName);
}

Value* Ident :: codegen(CodeGenContext &context)
{
	//cout << "Generating Ident..." << endl;
	auto &locals = context.getLocals();
	if (locals.find(name) == locals.end())
		return LogError("Ident: undeclared identifier: " + name);
	return context.Builder.CreateLoad(locals[name], name.c_str());
}

Value* Var :: codegen(CodeGenContext &context)
{
	//cout << "Generating Var..." << endl;
	auto &locals = context.getLocals();
	auto &types = context.getTypes();
	if (locals.find(name) == locals.end())
		return LogError("Var: undeclared identifier: " + name);
	//we do not init typeName in constrcutor, assign to it here!
	typeName = types[name]->getName();

	return context.Builder.CreateLoad(locals[name], name.c_str());
}

Value* ExpStmt :: codegen(CodeGenContext &context)
{
	//cout << "Generating ExpStmt..." << endl;
	if (!exp) return LogError("ExpStmt: empty exp");
	return exp->codegen(context);
}

Value* ReturnStmt :: codegen(CodeGenContext& context)
{
	//cout << "Generating ReturnStmt..." << endl;
	Value* returnValue = nullptr;
    if (exp){
    	returnValue = exp->codegen(context);
    	returnValue = removeRef(context, exp, returnValue);
    }
    context.setCurrentReturnValue(returnValue);
    return returnValue;
}

Value* FuncCall :: codegen(CodeGenContext &context)
{
	//cout << "Generating FuncCall..." << endl;
	//if funccall is for getarg/getargf do it as a macro
	if(globid->name == "getarg" || globid->name == "getargf"){
		auto value = callGetarg(context, args);
		//type converting
		if(value->getType()->isIntegerTy() && globid->name == "getargf")
			value = context.Builder.CreateSIToFP(value, Type::getFloatTy(context.TheContext));
		else if(value->getType()->isFloatTy() && globid->name == "getarg")
			value = context.Builder.CreateFPToSI(value, Type::getInt32Ty(context.TheContext));
		return value;
	}

	//start dealing normal function calls
	Function* function = context.module->getFunction(globid->name.c_str());
	if (!function)
		return LogError("FuncCall: empty function");
	if (function->arg_size() != args->size())
		return LogError("FuncCall: incorrect number of arguments");

	vector<Value*> params;
	Function::arg_iterator ait = function->arg_begin();

	if(args){
		auto &locals = context.getLocals();
		auto &types = context.getTypes();
		for(auto arg: *args){
			// we have to change var to ref if neccessary
			Type *t = (*(ait++)).getType();
			auto varPtr = dynamic_cast<Var*>(arg);
			if(t->isPointerTy() && varPtr && !types[varPtr->name]->getRef()){
				params.push_back(locals[varPtr->name]);
			}
			else if(!t->isPointerTy() && varPtr && types[varPtr->name]->getRef()){
				auto param = arg->codegen(context);
				params.push_back(removeRef(context, arg, param));
			}
			else{
				params.push_back(arg->codegen(context));
			}
		}
	}
	return context.Builder.CreateCall(function, makeArrayRef(params));
}

Value* Blk :: codegen(CodeGenContext &context)
{
	//cout << "Generating Blk..." << endl;
	Value* last = nullptr;
	for(auto stmt: statements){
		last = stmt->codegen(context);
	}
	return last;
}

Value* FuncDecl :: codegen(CodeGenContext &context)
{
	//cout << "Generating FuncDecl..." << endl;
	// prototype double(double, double, ...)
	vector<Type*> argTypes;
	VarList::const_iterator it;
	if (args)
		for (it = args->begin(); it != args->end(); it++)
			argTypes.push_back(typeOf(context, (*it)->getType()));

	FunctionType* funcType = FunctionType::get(typeOf(context, type), makeArrayRef(argTypes), false);
	Function* function = Function::Create(funcType, GlobalValue::InternalLinkage, globid->name.c_str(), context.module.get());

	BasicBlock* bblock = BasicBlock::Create(context.TheContext, "entry", function, nullptr);
	context.pushBlock(bblock);
	context.Builder.SetInsertPoint(bblock);

	auto &locals = context.getLocals();
	Function::arg_iterator ait = function->arg_begin();
	Value* argValue;
    if (args) {
    	int index = 1;
		for (it = args->begin(); it != args->end(); it++) {
			auto varName = (*it)->getVar()->name;
			(*it)->codegen(context);
			argValue = &*ait++;
			argValue->setName(varName.c_str());
			context.Builder.CreateStore(argValue, locals[varName]);
			//set noalias
			if((*it)->getType()->getNoalias())
				function->addAttribute(index, Attribute::NoAlias);
			++index;
		}
    }

	blk->codegen(context);
	context.Builder.CreateRet(context.getCurrentReturnValue());
	context.setCurrentReturnValue(nullptr);
	context.popBlock();
	return function;
}

Value* AssignStatementUtil(CodeGenContext &context, Var* lhs, Exp* rhs)
{
	//cout << "Generating AssignStatementUtil..." << endl;
	auto &locals = context.getLocals();
	auto &types = context.getTypes();

	lhs->typeName = types[lhs->name]->getName();	//only to set the lhs type
	Value* crhs = rhs->codegen(context);

	//if lhs is ref:
	Value *clhs;
	if(types[lhs->name]->getRef()){
		auto vrhs = dynamic_cast<Var*>(rhs);
		if(vrhs){ // if rhs is var
			if(types[vrhs->name]->getRef()){ // && if rhs is ref 
				auto tmp_load = context.Builder.CreateLoad(locals[vrhs->name]);
				return context.Builder.CreateStore(tmp_load, locals[lhs->name]);
			}
			else{	// && if rhs is not ref
				return context.Builder.CreateStore(locals[vrhs->name], locals[lhs->name]);
			}
		}
		else
			clhs = context.Builder.CreateLoad(locals[lhs->name]);
	}
	else
		clhs = locals[lhs->name];

	// else, 
	// remove reference if rhs is a Var and has ref type
	crhs = removeRef(context, rhs, crhs);

	// and do type convertion
	if (lhs->typeName.find("float") != string::npos && rhs->typeName.find("float") == string::npos) {
		crhs = context.Builder.CreateSIToFP(crhs, Type::getFloatTy(context.TheContext));
	} else if (lhs->typeName.find("int") != string::npos &&rhs->typeName.find("int") == string::npos) {
		crhs = context.Builder.CreateFPToSI(crhs, Type::getInt32Ty(context.TheContext));
	}

	return context.Builder.CreateStore(crhs, clhs);
}

Value* AssignStmt :: codegen(CodeGenContext &context)
{
	//cout << "Generating AssignStmt..." << endl;
	Value* alloc = vdecl->codegen(context);
	if (exp) 
		AssignStatementUtil(context, vdecl->getVar(), exp); // NAssignment_codeGen(vdecl->var, exp)

	return alloc;
}

Value* Extern :: codegen(CodeGenContext &context)
{
	//cout << "Generating Extern..." << endl;
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
	//cout << "Generating VarDecl..." << endl;
	AllocaInst* alloc = context.Builder.CreateAlloca(typeOf(context, type), nullptr, var->name.c_str());
	context.getLocals()[var->name] = alloc;
	context.getTypes()[var->name] = type;
	var->typeName = type->getName();
	return alloc;
}

Value* PrintStmt :: codegen(CodeGenContext &context)
{
	//cout << "Generating PrintStmt..." << endl;
	Function* function = context.module->getFunction("printf");
	Value* vstr;
	Value* vnum = exp->codegen(context);
	vnum = removeRef(context, exp, vnum);
	if (exp->typeName.find("int") != string::npos)
		vstr = context.Builder.CreateGlobalStringPtr("%d\n");
	else{
		vnum = context.Builder.CreateFPExt(vnum, Type::getDoubleTy(context.TheContext));
		vstr = context.Builder.CreateGlobalStringPtr("%f\n");
	}
	vector<Value*> int32_params{vstr, vnum};
	CallInst* call = context.Builder.CreateCall(function, int32_params, "print");
	return call;
}

Value* PrintSlitStmt :: codegen(CodeGenContext &context)
{
	//cout << "Generating PrintSlitStmt..." << endl;
	Function* function = context.module->getFunction("printf");
	string temp = str->name + "\n";
	Value *vstr = context.Builder.CreateGlobalStringPtr(temp);
	vector<Value*> int32_params{vstr};
	CallInst* call = context.Builder.CreateCall(function, int32_params, "printslit");
	return call;
}

Value* IfStmt :: codegen(CodeGenContext &context)
{
	//cout << "Generating IfStmt..." << endl;
	Value* condV = exp->codegen(context);
	if (!condV) return LogError("IfStmt: empty exp");

	condV = context.Builder.CreateICmpNE(condV,
		ConstantInt::get(Type::getInt32Ty(context.TheContext), 0, true));
	Function* function = context.getCurrentBlock()->getParent();

	BasicBlock* bbThen = BasicBlock::Create(context.TheContext, "then", function);
	BasicBlock* bbElse = BasicBlock::Create(context.TheContext, "else");
	BasicBlock* bbMerg = BasicBlock::Create(context.TheContext, "ifcon");

	context.Builder.CreateCondBr(condV, bbThen, bbElse);
	context.Builder.SetInsertPoint(bbThen);

	Value* thenV = stmt->codegen(context);

	if(context.getCurrentReturnValue()){
		context.Builder.CreateRet(context.getCurrentReturnValue());
		context.setCurrentReturnValue(nullptr);
	}
	else{
		context.Builder.CreateBr(bbMerg);
		bbThen = context.Builder.GetInsertBlock();
	}

	function->getBasicBlockList().push_back(bbElse);
	context.Builder.SetInsertPoint(bbElse);

	if (elsestmt)
		elsestmt->codegen(context);

	if(context.getCurrentReturnValue()){
		context.Builder.CreateRet(context.getCurrentReturnValue());
		context.setCurrentReturnValue(nullptr);
	}
	else{
		context.Builder.CreateBr(bbMerg);
		bbElse = context.Builder.GetInsertBlock();
	}

	function->getBasicBlockList().push_back(bbMerg);
	context.Builder.SetInsertPoint(bbMerg);

	return nullptr;
}

Value* BinOp :: codegen(CodeGenContext &context)
{
	//cout << "Generating BinOp..." << endl;
	if(op == "assign"){
		auto ret = AssignStatementUtil(context, dynamic_cast<Var*>(lhs), rhs);
		setTypeName();
		return ret;
	}

	Instruction::BinaryOps instBin;
	Instruction::OtherOps instOther;
	CmpInst::Predicate pred;

	Value* llhs = lhs->codegen(context);
	Value* lrhs = rhs->codegen(context);
	if (!llhs || !lrhs) return LogError("error: invalid binOp, one or more slide is empty.");

	setTypeName();
	// reference type
	llhs = removeRef(context, lhs, llhs);
	lrhs = removeRef(context, rhs, lrhs);

	// type conversion
	if (typeName.find("float") != string::npos) {	//return type is float/sfloat
		if (lhs->typeName.find("float") == string::npos)
			llhs = context.Builder.CreateSIToFP(llhs, Type::getFloatTy(context.TheContext));
		if (rhs->typeName.find("float") == string::npos)
			lrhs = context.Builder.CreateSIToFP(lrhs, Type::getFloatTy(context.TheContext));
	} else if (typeName.find("int") != string::npos) {	//return int/cint
		if (lhs->typeName.find("int") == string::npos)
			llhs = context.Builder.CreateFPToSI(llhs, Type::getInt32Ty(context.TheContext));
		if (rhs->typeName.find("int") == string::npos)
			lrhs = context.Builder.CreateFPToSI(lrhs, Type::getInt32Ty(context.TheContext));
	}

	if (op == "and") {
		instBin = Instruction::And;
		return context.Builder.CreateAnd(llhs, lrhs);
	} else if (op == "or") {
		instBin = Instruction::Or;
		return context.Builder.CreateOr(llhs, lrhs);
	}

	Value* value = nullptr;
	if (typeName.find("float") != string::npos) {
		if (op == "add") value = context.Builder.CreateFAdd(llhs, lrhs);
		else if (op == "sub") value = context.Builder.CreateFSub(llhs, lrhs);
		else if (op == "mul") value = context.Builder.CreateFMul(llhs, lrhs);
		else if (op == "div") value = context.Builder.CreateFDiv(llhs, lrhs);
		else if (op == "eq") value= context.Builder.CreateFCmpOEQ(llhs, lrhs);
		else if (op == "lt") value = context.Builder.CreateFCmpOLT(llhs, lrhs);
		else if (op == "gt") value = context.Builder.CreateFCmpOGT(llhs, lrhs);
		else throw runtime_error("unrecognized op: " + op);
		if (typeName == "float") {
			FastMathFlags fmf;
			fmf.setUnsafeAlgebra();
			((BinaryOperator*) value)->setFastMathFlags(fmf);
		}
	} else if (typeName == "int" || typeName == "cint") {
		if (op == "eq") value = context.Builder.CreateICmpEQ(llhs, lrhs);
		else if (op == "lt") value = context.Builder.CreateICmpSLT(llhs, lrhs);
		else if (op == "gt") value = context.Builder.CreateICmpSGT(llhs, lrhs);
		else {
			if (typeName == "int") {
				if (op == "add") value = context.Builder.CreateAdd(llhs, lrhs);
				else if (op == "sub") value = context.Builder.CreateSub(llhs, lrhs);
				else if (op == "mul") value = context.Builder.CreateMul(llhs, lrhs);
				else if (op == "div") value = context.Builder.CreateSDiv(llhs, lrhs);
				else throw runtime_error("unrecognized op: " + op);
			} else { // cint
				if (op == "div") {

					auto zero = make_shared<Num>("0");
					Value* isDividorZero = make_shared<BinOp>(rhs, "==", zero.get())->codegen(context);
					string errInfoDivBy0 = "'cint check: Div by 0!'";

					Value* errDivBy0 = context.Builder.CreateICmpNE(isDividorZero, 
						ConstantInt::get(Type::getInt32Ty(context.TheContext), 0, true));
					Function* function2 = context.getCurrentBlock()->getParent();
					BasicBlock* bbDvdBy0 = BasicBlock::Create(context.TheContext, "dvdbyzero", function2);
					BasicBlock* bbNormal2 = BasicBlock::Create(context.TheContext, "normal_2");
					BasicBlock* bbMerg2 = BasicBlock::Create(context.TheContext, "continue_2");

					context.Builder.CreateCondBr(errDivBy0, bbDvdBy0, bbNormal2);
					context.Builder.SetInsertPoint(bbDvdBy0);
					auto printError2 = make_shared<PrintSlitStmt>(errInfoDivBy0);
					printError2->codegen(context);
					context.Builder.CreateBr(bbMerg2);

					function2->getBasicBlockList().push_back(bbNormal2);
					context.Builder.SetInsertPoint(bbNormal2);
					context.Builder.CreateBr(bbMerg2);

					function2->getBasicBlockList().push_back(bbMerg2);
					context.Builder.SetInsertPoint(bbMerg2);
					//auto printError2 = make_shared<PrintSlitStmt>("Divided by 0!");
					//value = context.Builder.CreateSDiv(llhs, lrhs);



					Value* errDivOF = Intrinsic::getDeclaration(
						context.module.get(), Intrinsic::smul_with_overflow, Type::getInt32Ty(context.TheContext));
					string errInfoDivOF = "'cint check: Div operation overflows!'";

					Value *SBinaryOperatorWithOverflow = context.Builder.CreateCall(errDivOF, {llhs, lrhs}, "res");
					Value *SBinaryOperator = context.Builder.CreateExtractValue(SBinaryOperatorWithOverflow, 0, "binaryop");
					Value *Overflow = context.Builder.CreateExtractValue(SBinaryOperatorWithOverflow, 1, "obit");

					Function* function = context.getCurrentBlock()->getParent();
					BasicBlock* bbOverflow = BasicBlock::Create(context.TheContext, "overflow", function);
					BasicBlock* bbNormal = BasicBlock::Create(context.TheContext, "normal");
					BasicBlock* bbMerg = BasicBlock::Create(context.TheContext, "continue");

					context.Builder.CreateCondBr(Overflow, bbOverflow, bbNormal);
					context.Builder.SetInsertPoint(bbOverflow);
					auto printError = make_shared<PrintSlitStmt>(errInfoDivOF);
					printError->codegen(context);
					context.Builder.CreateBr(bbMerg);

					function->getBasicBlockList().push_back(bbNormal);
					context.Builder.SetInsertPoint(bbNormal);
					context.Builder.CreateBr(bbMerg);

					function->getBasicBlockList().push_back(bbMerg);
					context.Builder.SetInsertPoint(bbMerg);

					return SBinaryOperator;
				} else {
					Value* errV;
					string errInfo;
					if (op == "add") {
						errV = Intrinsic::getDeclaration(
							context.module.get(), Intrinsic::sadd_with_overflow, Type::getInt32Ty(context.TheContext));
						errInfo = "'cint check: Add operation overflows!'";
					} else if (op == "sub") {
						errV = Intrinsic::getDeclaration(
							context.module.get(), Intrinsic::ssub_with_overflow, Type::getInt32Ty(context.TheContext));
						errInfo = "'cint check: Sub operation overflows!'";
					} else if (op == "mul") {
						errV = Intrinsic::getDeclaration(
							context.module.get(), Intrinsic::smul_with_overflow, Type::getInt32Ty(context.TheContext));
						errInfo = "'cint check: Mul operation overflows!'";
					}
					else throw runtime_error("unrecognized op: " + op);
					auto *SBinaryOperatorWithOverflow = context.Builder.CreateCall(errV, {llhs, lrhs}, "res");
					auto *SBinaryOperator = context.Builder.CreateExtractValue(SBinaryOperatorWithOverflow, 0, "binaryop");
					auto *Overflow = context.Builder.CreateExtractValue(SBinaryOperatorWithOverflow, 1, "obit");

					Function* function = context.getCurrentBlock()->getParent();
					BasicBlock* bbOverflow = BasicBlock::Create(context.TheContext, "overflow", function);
					BasicBlock* bbNormal = BasicBlock::Create(context.TheContext, "normal");
					BasicBlock* bbMerg = BasicBlock::Create(context.TheContext, "continue");

					context.Builder.CreateCondBr(Overflow, bbOverflow, bbNormal);
					context.Builder.SetInsertPoint(bbOverflow);
					auto printError = make_shared<PrintSlitStmt>(errInfo);
					printError->codegen(context);
					context.Builder.CreateBr(bbMerg);

					function->getBasicBlockList().push_back(bbNormal);
					context.Builder.SetInsertPoint(bbNormal);
					context.Builder.CreateBr(bbMerg);

					function->getBasicBlockList().push_back(bbMerg);
					context.Builder.SetInsertPoint(bbMerg);
					return SBinaryOperator;
				}
			}
		}
	}
	//for eq gt and lt, the return type would be int1, convert it to int32*
	if(value->getType() == Type::getInt1Ty(context.TheContext)){
		value = context.Builder.CreateIntCast(value, Type::getInt32Ty(context.TheContext), true);
	}
	return value;
}

Value* UaryOp :: codegen(CodeGenContext &context)
{
	//cout << "Generating UaryOp..." << endl;
	shared_ptr<Num> zero = make_shared<Num>("0");
	shared_ptr<BinOp> bin = make_shared<BinOp>(zero.get(), op, exp);
	if (op == "-")
		return bin->codegen(context);
	else if (op == "!")
		return context.Builder.CreateNot(exp->codegen(context));
	return LogError("Invalid UaryOp.");
}

Value* WhileStmt :: codegen(CodeGenContext &context)
{
	//cout << "Generating WhileStmt..." << endl;
	Function* function = context.getCurrentBlock()->getParent();

	BasicBlock* bbCond = BasicBlock::Create(context.TheContext, "whilecond", function);
	BasicBlock* bbThen = BasicBlock::Create(context.TheContext, "loop");
	BasicBlock* bbMerg = BasicBlock::Create(context.TheContext, "whilecon");

	context.Builder.CreateBr(bbCond);
	context.Builder.SetInsertPoint(bbCond);
	Value* condV = exp->codegen(context);
	//and convert it to  bool(int1)
	condV = context.Builder.CreateICmpNE(condV,
		ConstantInt::get(Type::getInt32Ty(context.TheContext), 0, true));
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
	//cout << "Generating Prog..." << endl;

	createPrintf(context);

	if (externs)
		for(auto ext: *externs)
			ext->codegen(context);

	if(funcs)
		for(auto func: *funcs)
			func->codegen(context);

	return nullptr;
}
