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
#include <vector>
#include <string>


using VarTable = std::unordered_map<std::string, llvm::Value*>;
using TypeTable = std::unordered_map<std::string, NType*>;

class CodeGenBlock {
public:
    llvm::BasicBlock *block;
    llvm::Value *returnValue;
    VarTable locals;
    TypeTable types;
};

class CodeGenContext {
public:
    llvm::LLVMContext TheContext;
    llvm::IRBuilder<> Builder;
    std::unique_ptr<llvm::Module> module;
    
    CodeGenContext()
    : Builder(TheContext),
      module{std::unique_ptr<llvm::Module>(new llvm::Module{"main", TheContext})} {}

    void setOpt(bool to_opt) {
        opt = to_opt;
    }

    bool getOpt() {
        return opt;
    }

    void generateCode(Prog &root);

    // Print out all of the generated code.
    void printGenCode() {
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

    void setArgs(std::vector<std::string> const & _args){
        args = _args;
    }

    std::vector<std::string> getArgs(){
        return args;
    }
    
    llvm::BasicBlock *getCurrentBlock() {
        if (!blocks.empty()) {
            return blocks.top()->block;
        }
    	return nullptr;
    }
    
    void pushBlock(llvm::BasicBlock *block) {
        auto newBlock = std::make_shared<CodeGenBlock>();
        if(!blocks.empty()){
            newBlock->locals = blocks.top()->locals;
            newBlock->types = blocks.top()->types;
        }
        newBlock->block = block;
    	blocks.push(newBlock);
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
    std::stack<std::shared_ptr<CodeGenBlock>> blocks;
    std::vector<std::string> args;
    llvm::Function *mainFunction = nullptr;
    bool opt = false;
};


#endif