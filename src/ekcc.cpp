#include "util/util.h"
#include "node.h"
#include "parser/parser.hpp"
#include "parser/lexer.h"
#include "codegen/context.h"

#include <fstream>
#include <iostream>
#include <yaml-cpp/yaml.h>


extern Prog *rootProg;

int main(int argc, char **argv){
	auto args = setupArgs(argc, argv);

	int ret;
	try{
		auto ifs= std::ifstream(args.input);
		Lexer lexer(&ifs);

		auto parserPtr = new bison::Parser(lexer, *rootProg);
		parserPtr->parse();

		//check constraints
		if(rootProg)
			rootProg->check();

		if(args.ast){	//print ast
			YAML::Emitter out;
			if(rootProg)
				rootProg->printYaml(out);

			std::ofstream ofs(args.output);
			ofs << "---\n";
			ofs << out.c_str();
			ofs << "\n..." << std::endl;
		}
		else{
			llvm::InitializeNativeTarget();
			llvm::InitializeNativeTargetAsmPrinter();
			llvm::InitializeNativeTargetAsmParser();
			CodeGenContext context;
			context.setOpt(args.optimization);
			context.setJit(args.jit);
			context.generateCode(*rootProg);

			if(args.llvm){	//print ir
				if(args.output == "") //print to stdout
					context.printGenCode();
				else{	//write to file
					if (rootProg) {
						std::string os;
						llvm::raw_string_ostream ros(os);
						context.printGenCode(ros);
						auto tail = os.find("  ret void\n");
						os.insert(tail, "  call i32 @run()\n");
	            		std::ofstream ofs(args.output);
						ofs << os;
					}
				}
			}
			context.runCode();
		}

	} catch(std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		ret = 1;
	}

	return ret;
}