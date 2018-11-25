#include "node.h"
#include "context.h"

using namespace std;
using namespace llvm;

static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
static std::unique_ptr<Module> TheModule;
static std::map<std::string, Value *> NamedValues;
static std::map<std::string, std::string> NamedTypes;

static Value* ReturnValue;
static BasicBlock* CurrentBlock;

static Type *typeOf(NType *type) {
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
	if (typeName.find("int") != string::npos)
		return ConstantInt::get(Type::getInt32Ty(TheContext), stoi(val), true);
	else if (typeName.find("float") != string::npos)
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

Value* ExpStmt :: codegen()
{
	if (!exp) return LogError("ExpStmt: empty exp");
	return exp->codegen();
}

Value* ReturnStmt :: codegen()
{
	ReturnValue = nullptr;
	if (!exp) return LogError("ReturnStmt: empty exp");
	ReturnValue = exp->codegen();
	return ReturnValue;
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

	Function::arg_iterator ait = function->arg_begin();
	Value* argValue;
	if (args)
		for (it = args->begin(); it != args->end(); it++) {
			(**it).codegen();
			argValue = &*ait++;
			argValue->setName((*it)->getVar()->name.c_str());
			StoreInst* inst = Builder.CreateStore(argValue, NamedValues[(*it)->getVar()->name]);
		}

	blk->codegen();
	Builder.CreateRet(ReturnValue);
	return function;
}

Value* AssignStatementUtil(Exp* hs, Exp* rhs)
{
	Var* lhs = (Var*) hs;
	if (NamedValues.find(lhs->name) == NamedValues.end())
		return nullptr;

	Value* assign_gen = rhs->codegen();
	Value* assign_rhs;

	if (lhs->typeName.find("float") != string::npos) {
		if (rhs->typeName.find("float") != string::npos)
			assign_rhs = assign_gen;
		else
			assign_rhs = Builder.CreateSIToFP(assign_gen, Type::getFloatTy(TheContext), "");
	} else if (lhs->typeName.find("int") != string::npos) {
		if (rhs->typeName.find("int") != string::npos)
			assign_rhs = assign_gen;
		else
			assign_rhs = Builder.CreateFPToSI(assign_gen, Type::getInt32Ty(TheContext), "");
	} else {
		assign_rhs = assign_gen;
	}

	Value* clhs = assign_rhs;
	string temp = NamedTypes[lhs->name];
	if (temp.find("ref int") != string::npos || temp.find("ref cint") != string::npos ||
		temp.find("ref float") != string::npos || temp.find("ref sfloat") != string::npos) {
		clhs = Builder.CreateLoad(NamedValues[lhs->name], "");
		return Builder.CreateStore(assign_rhs, clhs);
	} else {
		return Builder.CreateStore(clhs, NamedValues[lhs->name]);
	}

	return nullptr;
}

Value* AssignStmt::codegen()
{
	Value* alloc = vdecl->codegen();
	if (exp) AssignStatementUtil(vdecl->getVar(), exp); // NAssignment_codeGen(vdecl->var, exp)

	return alloc;
}

Value* Extern::codegen()
{
	vector<Type*> types;
	for (auto it = (*args).begin(); it != (*args).end(); it++)
		types.push_back(typeOf(*it));
	FunctionType* funcType = FunctionType::get(typeOf(type), makeArrayRef(types), false);
	Function* function = Function::Create(funcType, GlobalValue::ExternalLinkage, globid->name.c_str(), TheModule.get());
	return function;
}


Value* VarDecl::codegen()
{
	AllocaInst* alloc = Builder.CreateAlloca(typeOf(type), 0, var->name.c_str());
	NamedValues[var->name] = alloc;
	NamedTypes[var->name] = type->getName();
	return alloc;
}

Value* PrintStmt::codegen()
{
	Function* function = TheModule.get()->getFunction("printf");
	Value* vstr;
	if (exp->typeName.find("int") != string::npos)
		vstr = Builder.CreateGlobalStringPtr("%d");
	else
		vstr = Builder.CreateGlobalStringPtr("%f");
	vector<Value*> int32_params;
	int32_params.push_back(vstr);
	int32_params.push_back(exp->codegen());
	CallInst* call = Builder.CreateCall(function, int32_params, "");
	return call;
}

Value* PrintSlitStmt::codegen()
{
	Function* function = TheModule.get()->getFunction("printf");
	string temp = str->name.substr(1, str->name.size() - 2);
	Value *vstr = Builder.CreateGlobalStringPtr(temp);
	vector<Value*> int32_params;
	int32_params.push_back(vstr);
	CallInst* call = Builder.CreateCall(function, int32_params, "");
	return call;
}

Value* IfStmt::codegen()
{
	Value* condV = exp->codegen();
	if (!condV) return LogError("IfStmt: empty exp");

	condV = Builder.CreateICmpNE(condV, Builder.getInt1(0), "");
	Function* function = CurrentBlock->getParent();

	BasicBlock* bbThen = BasicBlock::Create(TheContext, "then", function);
	BasicBlock* bbElse = BasicBlock::Create(TheContext, "else");
	BasicBlock* bbMerg = BasicBlock::Create(TheContext, "ifcon");

	Builder.CreateCondBr(condV, bbThen, bbElse);
	Builder.SetInsertPoint(bbThen);

	Value* thenV = stmt->codegen();

	Builder.CreateBr(bbMerg);
	bbThen = Builder.GetInsertBlock();

	function->getBasicBlockList().push_back(bbElse);
	Builder.SetInsertPoint(bbElse);

	Value* elseV = nullptr;
	if (elsestmt) elseV = elsestmt->codegen();

	Builder.CreateBr(bbMerg);
	bbElse = Builder.GetInsertBlock();

	function->getBasicBlockList().push_back(bbMerg);
	Builder.SetInsertPoint(bbMerg);

	return nullptr;
}

Value* BinOp :: codegen()
{
	Instruction::BinaryOps instBin;
	Instruction::OtherOps instOther;
	CmpInst::Predicate pred;

	Value* clhs = lhs->codegen();
	Value* crhs = rhs->codegen();
	if (!clhs || !crhs) return nullptr;

	if (op == "&&") {
		instBin = Instruction::And;
		return Builder.CreateAnd(clhs, crhs, "");
	} else if (op == "||") {
		instBin = Instruction::Or;
		return Builder.CreateOr(clhs, crhs, "");
	} else if (op == "=") {
		return AssignStatementUtil(lhs, rhs);
	}

	// reference type
	//

	// tpye conversion
	Value* llhs, * rrhs;
	if (typeName.find("float") != string::npos) {
		if (lhs->typeName.find("float") != string::npos)
			llhs = clhs;
		else
			llhs = Builder.CreateSIToFP(clhs, Type::getFloatTy(TheContext), "");
		if (rhs->typeName.find("float") != string::npos)
			rrhs = crhs;
		else
			rrhs = Builder.CreateSIToFP(crhs, Type::getFloatTy(TheContext), "");
	} else if (typeName.find("int") != string::npos) {
		if (lhs->typeName.find("int") != string::npos)
			llhs = clhs;
		else
			llhs = Builder.CreateFPToSI(clhs, Type::getInt32Ty(TheContext), "");
		if (rhs->typeName.find("int") != string::npos)
			rrhs = crhs;
		else
			rrhs = Builder.CreateFPToSI(crhs, Type::getInt32Ty(TheContext), "");
	} else {
		llhs = clhs;
		rrhs = crhs;
	}

	Value* value;
	if (typeName == "float" || typeName == "sfloat") {
		if (op == "+") value = Builder.CreateFAdd(llhs, rrhs, "");
		else if (op == "-") value = Builder.CreateFSub(llhs, rrhs, "");
		else if (op == "*") value = Builder.CreateFMul(llhs, rrhs, "");
		else if (op == "/") value = Builder.CreateFDiv(llhs, rrhs, "");
		else if (op == "==") value= Builder.CreateFCmpOEQ(llhs, rrhs, "");
		else if (op == "<") value = Builder.CreateFCmpOLT(llhs, rrhs, "");
		else if (op == ">") value = Builder.CreateFCmpOGT(llhs, rrhs, "");
		if (typeName == "float") {
			FastMathFlags fmf;
			fmf.setUnsafeAlgebra();
			((BinaryOperator*) value)->setFastMathFlags(fmf);
		}
		return value;
	} else if (typeName == "int" || typeName == "cint") {
		if (op == "==") value = Builder.CreateICmpEQ(llhs, rrhs, "");
		else if (op == "<") value = Builder.CreateICmpSLT(llhs, rrhs, "");
		else if (op == ">") value = Builder.CreateICmpSGT(llhs, rrhs, "");
		else {
			if (typeName == "int") {
				if (op == "+") value = Builder.CreateAdd(llhs, rrhs, "");
				else if (op == "-") value = Builder.CreateSub(llhs, rrhs, "");
				else if (op == "*") value = Builder.CreateMul(llhs, rrhs, "");
				else if (op == "/") value = Builder.CreateSDiv(llhs, rrhs, "");
			} else { // cint
				if (op == "/")
					value = Builder.CreateSDiv(llhs, rrhs, "");
				else {
					Value* errV;
					string errInfo;
					if (op == "+") {
						errV = Intrinsic::getDeclaration(
							TheModule.get(), Intrinsic::sadd_with_overflow, Type::getInt32Ty(TheContext));
						errInfo = ",Add operation overflows!,";
					} else if (op == "-") {
						errV = Intrinsic::getDeclaration(
							TheModule.get(), Intrinsic::ssub_with_overflow, Type::getInt32Ty(TheContext));
						errInfo = ",Sub operation overflows!,";
					} else if (op == "*") {
						errV = Intrinsic::getDeclaration(
							TheModule.get(), Intrinsic::smul_with_overflow, Type::getInt32Ty(TheContext));
						errInfo = ",Mul operation overflows!,";
					}
					auto *SBinaryOperatorWithOverflow = Builder.CreateCall(errV, {llhs, rrhs}, "t");
					auto *SBinaryOperator = Builder.CreateExtractValue(SBinaryOperatorWithOverflow, 0, "binaryop");
					auto *Overflow = Builder.CreateExtractValue(SBinaryOperatorWithOverflow, 1, "obit");

					Function* function = CurrentBlock->getParent();
					BasicBlock* bbNormal = BasicBlock::Create(TheContext, "normal", function);
					BasicBlock* bbOverflow = BasicBlock::Create(TheContext, "overflow", function);
					BasicBlock* bbMerg = BasicBlock::Create(TheContext, "continue");

					Builder.CreateCondBr(Overflow, bbNormal, bbOverflow);
					Builder.SetInsertPoint(bbNormal);
					Builder.CreateBr(bbMerg);

					function->getBasicBlockList().push_back(bbOverflow);
					Builder.SetInsertPoint(bbOverflow);
					auto printError = make_shared<PrintSlitStmt>(errInfo);
					printError.get()->codegen();
					Builder.CreateBr(bbMerg);

					function->getBasicBlockList().push_back(bbMerg);
					Builder.SetInsertPoint(bbMerg);
					return SBinaryOperator;
				}
			}
		}
		return value;
	}
	return nullptr;
}

Value* UaryOp::codegen()
{
	shared_ptr<Num> zero = make_shared<Num>("0");
	shared_ptr<BinOp> bin = make_shared<BinOp>(zero.get(), op, exp);
	if (op.find("-") != string::npos)
		return bin->codegen();
	else if (op.find("!") != string::npos)
		return Builder.CreateNot(exp->codegen(), "");
	return nullptr;
}

Value* WhileStmt::codegen()
{
	Function* function = CurrentBlock->getParent();

	BasicBlock* bbCond = BasicBlock::Create(TheContext, "whilecond", function);
	BasicBlock* bbThen = BasicBlock::Create(TheContext, "loop");
	BasicBlock* bbMerg = BasicBlock::Create(TheContext, "whilecon");

	Builder.CreateBr(bbCond);
	Builder.SetInsertPoint(bbCond);
	Value* condV = exp->codegen();
	Builder.CreateCondBr(condV, bbThen, bbMerg);

	function->getBasicBlockList().push_back(bbThen);
	Builder.SetInsertPoint(bbThen);
	Value* thenV = stmt->codegen();

	Builder.CreateBr(bbCond);
	bbThen = Builder.GetInsertBlock();

	function->getBasicBlockList().push_back(bbMerg);
	Builder.SetInsertPoint(bbMerg);
	return nullptr;
}

Value* Prog::codegen()
{
	if (externs)
		for(auto ext: *externs)
			ext->codegen();
	for(auto func: *funcs)
		func->codegen();
	return nullptr;
}
