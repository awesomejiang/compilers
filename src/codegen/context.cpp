#include "context.h"

#include <vector>
#include <utility>
#include <string>

using namespace std;
using namespace llvm;

void CodeGenContext::generateCode(Prog& root) {
    if (getOpt()) {
        cout << "Enabling optimization..." << endl;
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

    cout << "Generating code..." << endl;

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

}

GenericValue CodeGenContext::runCode() {
    cout << "Running code with JIT!" << endl;

    //no args
    vector<GenericValue> noargs;

    Function* runFunction = cast<Function>(module->getFunction("run"));
    if (!runFunction) {
        cout << "run() does not exist!" << endl;
    }

    string err;
    auto eb = make_shared<EngineBuilder>(move(module));
    eb->setEngineKind(EngineKind::JIT).setErrorStr(&err);
    auto ee = eb->setErrorStr(&err).create();
    if (!ee) {
        cout << "error creating ExecutionEngine: " + err << endl;
    }

    ee->finalizeObject();

    GenericValue v = ee->runFunction(runFunction, noargs);
    cout << "Code was run." << endl;
    return v;
}