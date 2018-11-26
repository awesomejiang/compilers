#include "context.h"

#include <vector>
#include <utility>

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

    cout << "Code was generated." << endl;
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

GenericValue CodeGenContext::runCode() {
    cout << "Running code..." << endl;
    if(getJit())
        cout << "...With JIT!" << endl;
    
    vector<GenericValue> noargs;
    Function* runFunction = cast<Function>(module->getFunction("run"));
    if (!runFunction) {
        cout << "run() does not exist!" << endl;
    }
    string err;
    auto eb = make_shared<EngineBuilder>(move(module));
    if (getJit()) {
        eb->setEngineKind(EngineKind::JIT).setErrorStr(&err);
    }
    auto ee = eb->setErrorStr(&err).create();

    if (!ee) {
        cout << "error creating ExecutionEngine: " + err << endl;
    }

    ee->finalizeObject();

    GenericValue v = ee->runFunction(runFunction, noargs);
    cout << "Code was run." << endl;
    return v;
}