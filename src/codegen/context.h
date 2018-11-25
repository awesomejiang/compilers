#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Bitcode/BitstreamReader.h>
#include <llvm/Bitcode/BitstreamWriter.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/Interpreter.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/IPO.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>

#include "node.h"
#include "parser.hpp"

#include <memory>
#include <stack>
#include <typeinfo>
#include <unordered_map>


using std::unordered_map<std::string, std::shared_ptr<llvm::Value>> VarTable;
using std::unordered_map<std::string, std::string> TypeTable;

class CodeGenBlock {
public:
    std::shared_ptr<llvm::BasicBlock> block;
    std::shared_ptr<llvm::Value> returnValue;
    VarTable locals;
    TypeTable types;
};

class CodeGenContext {
public:
    std::shared_ptr<llvm::Module> module;
    
    CodeGenContext(): module{std::make_shared<llvm::Module>("main", TheContext)}, Builder(TheContext) {}

    void setOpt(bool to_opt) {
        opt = to_opt;
    }

    bool getOpt() {
        return opt;
    }

    void setJit(bool to_jit) {
        jit = to_jit;
    }

    bool getJit() {
        return jit;
    }

    void generateCode(Prog &root);

    // Print out all of the generated code.
    void printGenCode() {
        std::cout << "Printing generated code..." << std::endl;
        module->print(llvm::errs(), nullptr);
    };
    
    void printGenCode(llvm::raw_ostream &os) {
        module->print(os, nullptr);
    }
    
    llvm::GenericValue runCode();

    VarTable& getLocals() {
        return blocks.top()->locals ;
    }
    
    TypeTable& getTypes() {
        return blocks.top()->types ;
    }
    
    llvm::BasicBlock *getCurrentBlock() {
        if (!blocks.empty()) {
            return blocks.top()->block;
        }
    	return nullptr;
    }
    
    void pushBlock(BasicBlock *block) {
    	blocks.push(std::make_shared<CodeGenBlock>());
    	blocks.top()->block = std::make_shared(*block);
    }
    
    void popBlock() {
    	blocks.pop();
    }

    void setCurrentReturnValue(llvm::Value *value) {
        if (!blocks.empty()) {
            blocks.top()->returnValue = value;
        }
    }

    llvm::Value* getCurrentReturnValue() {
        if (!blocks.empty()) {
            return blocks.top()->returnValue;
        }
        return nullptr;
    }
private:
    llvm::LLVMContext TheContext;
    llvm::IRBuilder<> Builder;
    std::stack<CodeGenBlock *> blocks;
    llvm::Function *mainFunction = nullptr;
    bool opt = false;
    bool jit = false;
};
#endif