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

static void createPrintfFunction(CodeGenContext& context) {
    vector<Type*> printfArgTypes;
    printfArgTypes.push_back(Type::getInt8PtrTy(context.TheContext)); //char*

    FunctionType* retType = FunctionType::get(Type::getInt32Ty(context.TheContext), printfArgTypes, true);

    Function *func = Function::Create(
                retType, Function::ExternalLinkage,
                Twine("printf"),
                context.module.get()
           );
    func->setCallingConv(CallingConv::C);
}


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
	auto locals = context.getLocals();
	if (locals.find(name) == locals.end())
		return LogError("Ident: undeclared identifier: " + name);
	return context.Builder.CreateLoad(locals[name], name.c_str());
}

Value* Var :: codegen(CodeGenContext &context)
{
	//cout << "Generating Var..." << endl;
	auto locals = context.getLocals();
	if (locals.find(name) == locals.end())
		return LogError("Var: undeclared identifier: " + name);
	typeName = context.getTypes()[name]->getName();	//we do not init typeName in constrcutor
	if(typeName == "")
		throw runtime_error("error: var do not found in symbol table.");
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
    if (!exp) return LogError("ReturnStmt: empty exp");
    Value* returnValue = exp->codegen(context);
    context.setCurrentReturnValue(returnValue);
    return returnValue;
}

Value* FuncCall :: codegen(CodeGenContext &context)
{
	//cout << "Generating FuncCall..." << endl;
	Function* function = context.module->getFunction(globid->name.c_str());
	if (!function)
		return LogError("FuncCall: empty function");
	if (function->arg_size() != args->size())
		return LogError("FuncCall: incorrect number of arguments");

	vector<Value*> params;
	Function::arg_iterator ait = function->arg_begin();
	if(args)
		for(auto arg: *args){
			auto param = arg->codegen(context);
			// we have to change var to ref if neccessary
			Type *t = (*(ait++)).getType();
			if(t->isPointerTy() && !param->getType()->isPointerTy()){
				AllocaInst* alloc = context.Builder.CreateAlloca(param->getType(), nullptr);
				context.Builder.CreateStore(param, alloc);
				params.push_back(alloc);
			}
			else
				params.push_back(param);
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
			locals[varName] = context.Builder.CreateAlloca(argValue->getType(), nullptr, varName.c_str());
			context.Builder.CreateStore(argValue, locals[varName]);
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
	//cout << "Generating AssignStatementUtil..." << endl;
	auto locals = context.getLocals();
	auto types = context.getTypes();

	Var* lhs = dynamic_cast<Var*>(hs);
	if (locals.find(lhs->name) == locals.end())
		return nullptr;

	Value* assign_gen = rhs->codegen(context);

	// reference type if rhs is a Var and has ref type
	auto rhs_ptr = dynamic_cast<Var*>(rhs);
	if(rhs_ptr && types[rhs_ptr->name]->getRef())
		assign_gen = context.Builder.CreateLoad(assign_gen);

	Value* assign_rhs;
	if (lhs->typeName.find("float") != string::npos) {
		if (rhs->typeName.find("float") != string::npos)
			assign_rhs = assign_gen;
		else
			assign_rhs = context.Builder.CreateSIToFP(assign_gen, Type::getFloatTy(context.TheContext));
	} else if (lhs->typeName.find("int") != string::npos) {
		if (rhs->typeName.find("int") != string::npos)
			assign_rhs = assign_gen;
		else
			assign_rhs = context.Builder.CreateFPToSI(assign_gen, Type::getInt32Ty(context.TheContext));
	} else {
		assign_rhs = assign_gen;
	}

	Value* crhs = assign_rhs;
	auto type = context.getTypes()[lhs->name];
	if (type->getRef()) {
		crhs = context.Builder.CreateLoad(locals[lhs->name]);
		return context.Builder.CreateStore(assign_rhs, crhs);
	} else {
		return context.Builder.CreateStore(crhs, locals[lhs->name]);
	}

	return nullptr;
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
	return alloc;
}

Value* PrintStmt :: codegen(CodeGenContext &context)
{
	//cout << "Generating PrintStmt..." << endl;
	Function* function = context.module->getFunction("printf");
	Value* vstr;
	if (exp->typeName.find("int") != string::npos)
		vstr = context.Builder.CreateGlobalStringPtr("%d");
	else
		vstr = context.Builder.CreateGlobalStringPtr("%f");
	vector<Value*> int32_params{vstr, exp->codegen(context)};
	CallInst* call = context.Builder.CreateCall(function, int32_params);
	return call;
}

Value* PrintSlitStmt :: codegen(CodeGenContext &context)
{
	//cout << "Generating PrintSlitStmt..." << endl;
	Function* function = context.module->getFunction("printf");
	string temp = str->name;
	Value *vstr = context.Builder.CreateGlobalStringPtr(temp);
	vector<Value*> int32_params{vstr};
	CallInst* call = context.Builder.CreateCall(function, int32_params);
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
	//cout << "Generating BinOp..." << endl;
	Instruction::BinaryOps instBin;
	Instruction::OtherOps instOther;
	CmpInst::Predicate pred;

	Value* clhs = lhs->codegen(context);
	Value* crhs = rhs->codegen(context);
	setTypeName();
	if (!clhs || !crhs) return nullptr;

	if (op == "and") {
		instBin = Instruction::And;
		return context.Builder.CreateAnd(clhs, crhs);
	} else if (op == "or") {
		instBin = Instruction::Or;
		return context.Builder.CreateOr(clhs, crhs);
	} else if (op == "assign") {
		return AssignStatementUtil(context, lhs, rhs);
	}

	// reference type
	auto types = context.getTypes();
	auto lhs_ptr = dynamic_cast<Var*>(lhs);
	if(lhs_ptr && types[lhs_ptr->name]->getRef())
		clhs = context.Builder.CreateLoad(clhs);
	auto rhs_ptr = dynamic_cast<Var*>(rhs);
	if(rhs_ptr && types[rhs_ptr->name]->getRef())
		crhs = context.Builder.CreateLoad(crhs);

	// tpye conversion
	Value* llhs, * rrhs;
	if (typeName == "float") {
		if (lhs->typeName == "float")
			llhs = clhs;
		else
			llhs = context.Builder.CreateSIToFP(clhs, Type::getFloatTy(context.TheContext));
		if (rhs->typeName == "float")
			rrhs = crhs;
		else
			rrhs = context.Builder.CreateSIToFP(crhs, Type::getFloatTy(context.TheContext));
	} else if (typeName == "int") {
		if (lhs->typeName == "int")
			llhs = clhs;
		else
			llhs = context.Builder.CreateFPToSI(clhs, Type::getInt32Ty(context.TheContext));
		if (rhs->typeName == "int")
			rrhs = crhs;
		else
			rrhs = context.Builder.CreateFPToSI(crhs, Type::getInt32Ty(context.TheContext));
	} else {
		llhs = clhs;
		rrhs = crhs;
	}

	Value* value = nullptr;
	if (typeName == "float" || typeName == "sfloat") {
		if (op == "add") value = context.Builder.CreateFAdd(llhs, rrhs);
		else if (op == "sub") value = context.Builder.CreateFSub(llhs, rrhs);
		else if (op == "mul") value = context.Builder.CreateFMul(llhs, rrhs);
		else if (op == "div") value = context.Builder.CreateFDiv(llhs, rrhs);
		else if (op == "eq") value= context.Builder.CreateFCmpOEQ(llhs, rrhs);
		else if (op == "lt") value = context.Builder.CreateFCmpOLT(llhs, rrhs);
		else if (op == "gt") value = context.Builder.CreateFCmpOGT(llhs, rrhs);
		else throw runtime_error("unrecognized op: " + op);
		if (typeName == "float") {
			FastMathFlags fmf;
			fmf.setUnsafeAlgebra();
			((BinaryOperator*) value)->setFastMathFlags(fmf);
		}
	} else if (typeName == "int" || typeName == "cint") {
		if (op == "eq") value = context.Builder.CreateICmpEQ(llhs, rrhs);
		else if (op == "lt") value = context.Builder.CreateICmpSLT(llhs, rrhs);
		else if (op == "gt") value = context.Builder.CreateICmpSGT(llhs, rrhs);
		else {
			if (typeName == "int") {
				if (op == "add") value = context.Builder.CreateAdd(llhs, rrhs);
				else if (op == "sub") value = context.Builder.CreateSub(llhs, rrhs);
				else if (op == "mul") value = context.Builder.CreateMul(llhs, rrhs);
				else if (op == "div") value = context.Builder.CreateSDiv(llhs, rrhs);
				else throw runtime_error("unrecognized op: " + op);
			} else { // cint
				if (op == "div")
					value = context.Builder.CreateSDiv(llhs, rrhs);
				else {
					Value* errV;
					string errInfo;
					if (op == "add") {
						errV = Intrinsic::getDeclaration(
							context.module.get(), Intrinsic::sadd_with_overflow, Type::getInt32Ty(context.TheContext));
						errInfo = ",Add operation overflows!,";
					} else if (op == "sub") {
						errV = Intrinsic::getDeclaration(
							context.module.get(), Intrinsic::ssub_with_overflow, Type::getInt32Ty(context.TheContext));
						errInfo = ",Sub operation overflows!,";
					} else if (op == "mul") {
						errV = Intrinsic::getDeclaration(
							context.module.get(), Intrinsic::smul_with_overflow, Type::getInt32Ty(context.TheContext));
						errInfo = ",Mul operation overflows!,";
					}
					else throw runtime_error("unrecognized op: " + op);
					auto *SBinaryOperatorWithOverflow = context.Builder.CreateCall(errV, {llhs, rrhs});
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
	if (op.find("-") != string::npos)
		return bin->codegen(context);
	else if (op.find("!") != string::npos)
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

	createPrintfFunction(context);

	if (externs)
		for(auto ext: *externs)
			ext->codegen(context);

	if(funcs)
		for(auto func: *funcs)
			func->codegen(context);

	return nullptr;
}
