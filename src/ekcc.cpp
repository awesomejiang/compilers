#include "util/util.h"
#include "parser/parser.hpp"
#include "parser/lexer.h"

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
	} catch(std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		ret = 1;
	}

	return ret;
}
