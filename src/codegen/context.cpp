#include "context.h"

#include <vector>

using namespace std;
using namespace llvm;

void CodeGenContext::generateCode(Prog& root) {
    cout << "Generating code..." << endl;
    Timer t("codeGenerator");

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
        cout << "Enabling optimization..." << endl;
        PassManagerBuilder builder;
        auto pm = make_shared<legacy::PassManager>();
        auto fm = make_shared<legacy::FunctionPassManager>(module.get());

        builder.OptLevel = optType.level3? 3: 0;
        builder.SizeLevel = 0;
        if(optType.level3 || optType.inlining)
            builder.Inliner = createFunctionInliningPass(3, 0, true);
        builder.DisableUnitAtATime = optType.level3? false: true;
        builder.DisableUnrollLoops = optType.level3? false: true;
        builder.LoopVectorize = optType.level3? true: false;
        builder.SLPVectorize = optType.level3? true: false;
        builder.MergeFunctions = optType.unrolling;
        builder.populateFunctionPassManager(*fm);
        builder.populateModulePassManager(*pm);

        fm->doInitialization();
        for (Function &f : *module)
            fm->run(f);
        fm->doFinalization();

//        unique_ptr<Module> m2(CloneModule(module.get()));
//        pm->run(*module);
//        module = move(m2);
    }
}

GenericValue CodeGenContext::runCode() {
    cout << "Running code with JIT!" << endl;
        Timer t("jit");

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