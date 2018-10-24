#include "util.h"

CompileException::CompileException(std::string const &msg,
  std::string const &file,
  std::string const &func)
: std::runtime_error(msg.c_str()), file{file}, func{func} {}

void CompileException::printError() const{
	std::cerr << "Exception thrown by" << func << "() in "
		<< file << ": " << what() << std::endl;
}


CompileArgs setupArgs(int argc, char **argv){
	CompileArgs args;

	for(auto i=1; i<argc; ++i){
		std::string arg{argv[i]};

		if(arg == "-h" || arg == "-?"){	//print help
			printUsage();
			exit(0);
		}
		else if(arg == "-v"){	//additional messages
			args.verbose = true;
		}
		else if(arg == "-O"){	//optimizations
			args.optimization = true;
		}
		else if(arg == "-emit-ast"){	//print serialized AST in output file
			args.ast = true;
		}
		else if(arg == "-emit-llvm"){	//produce LLVM IR
			args.llvm = true;
		}
		else if(arg == "-o"){	//output and input file, required
			args.output = argv[i+1];
			args.input = argv[i+2];
			i += 2;
		}
		else{
			printUsage();
			exit(0);
		}
	}

	if(args.output == "" || args.output == ""){
		printUsage();
		exit(0);
	}

	return args;
}


void printUsage(){
	std::cout << "E-K Compiler by Jiawei Jiang" << std::endl;
	std::cout << "Usage: ./bin/ekcc[.py] [-h|-?] [-v] [-O] [-emit-ast|-emit-llvm] -o <output-file> <input-file>" << std::endl;
	std::cout << "OPtions:" << std::endl;
	std::cout << "	[-h|-?]                 call usage" << std::endl;
	std::cout << "	[-v]                    print extra messages" << std::endl;
	std::cout << "	[-O]                    enable optimizations" << std::endl;
	std::cout << "	[-emit-ast|-emit-llvm]  print AST/LLVM IR in output file" << std::endl;
	std::cout << "	-o <output> <input>     path of output and input file" << std::endl;
}