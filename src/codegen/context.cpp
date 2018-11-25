void generateCode(Prog &root);
llvm::GenericValue runCode();
#include <vector>

using namespace std;
using namespace llvm;

extern LLVMContext TheContext;

void CodeGenContext::generateCode(Prog& root) {
    cout << "Generating code..." << endl;
    
    /* Create the top level interpreter function to call as entry */
    vector<Type*> argTypes;
    FunctionType *ftype = FunctionType::get(Type::getVoidTy(TheContext), makeArrayRef(argTypes), false);
    mainFunction = Function::Create(ftype, GlobalValue::InternalLinkage, "main", module.get());
    BasicBlock *bblock = BasicBlock::Create(TheContext, "entry", mainFunction, 0);
    /*std::vector<Value*> args;
    ExpressionList::const_iterator it;
    CallInst *call = CallInst::Create(function, makeArrayRef(args), "", context.currentBlock());*/
    
    /* Push a new variable/block context */
    pushBlock(bblock);
    root.codeGen(); /* emit bytecode for the toplevel block */
    ReturnInst::Create(MyContext, bblock);
    popBlock();

    /* Print the bytecode in a human-readable format
     to see if our program compiled properly
     */
    std::cout << "Code is generated.\n";
    // module->dump();

    if (getOpt()) {
        std::cout << "Running optimization...\n";
        // legacy::PassManager pm;
        // pm.add(createPrintModulePass(outs()));
        // pm.run(*module);
        llvm::legacy::PassManager *pm = new llvm::legacy::PassManager();
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
        delete pm;
    }
}