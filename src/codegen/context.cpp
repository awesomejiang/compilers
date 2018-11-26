#include "context.h"

#include <vector>

using namespace std;
using namespace llvm;

void CodeGenContext::generateCode(Prog& root) {
    cout << "Generating code..." << endl;
    
    // Create the top level interpreter function to call as entry
    vector<Type*> argTypes;
    FunctionType *ftype = FunctionType::get(Type::getVoidTy(TheContext), makeArrayRef(argTypes), false);
    mainFunction = Function::Create(ftype, GlobalValue::InternalLinkage, "main", module.get());
    BasicBlock *bblock = BasicBlock::Create(TheContext, "entry", mainFunction, 0);

    // Run code generator from root
    pushBlock(bblock);
    root.codegen(*this);
    ReturnInst::Create(TheContext, bblock);
    popBlock();

    cout << "Code is generated." << endl;
    // module->dump();

    if (getOpt()) {
        cout << "Running optimization..." << endl;
        auto pm = make_shared<legacy::PassManager>();
        int optLevel = 3;
        int sizeLevel = 0;
        PassManagerBuilder builder;
        builder.OptLevel = optLevel;
        builder.SizeLevel = sizeLevel;
        builder.Inliner = createFunctionInliningPass(optLevel, sizeLevel, true);
        builder.DisableUnitAtATime = false;
        builder.DisableUnrollLoops = false;
        builder.LoopVectorize = true;
        builder.SLPVectorize = true;
        builder.populateModulePassManager(*pm);
        pm->run(*module);
    }
}


/* Executes the AST by running the main function */
GenericValue CodeGenContext::runCode() {
    cout << (getJit() ? "JIT " : "") << "Running code..." << endl;
    vector<GenericValue> noargs;
    Function* TheRunF = cast<Function>(module->getFunction("run"));
    if (TheRunF == nullptr) {
        cout << "run() does not exist!" << endl;
    }
    std::string err;
    auto eb = make_shared<EngineBuilder>(std::unique_ptr<Module>(module.get()));
    if (getJit()) {
        eb->setEngineKind(EngineKind::JIT).setErrorStr(&err);
    }
    ExecutionEngine *ee = eb->create();

    if (!ee) {
        cout << "Error while creating ExecutionEngine in runCode()!" << endl;
    }

    ee->finalizeObject();
    GenericValue v = ee->runFunction(TheRunF, noargs);
    cout << "Code is run." << endl;
    return v;
}