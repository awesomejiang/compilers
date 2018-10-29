#include "util/util.h"
#include "parser/parser.hpp"
#include "parser/lexer.h"
#include <stdint.h>
#include <stddef.h>

#include <string>
#include <sstream>
#include <iostream>
#include <yaml-cpp/yaml.h>

extern Prog *rootProg;

void parseFile(const uint8_t *data, size_t size){
	auto ss= std::stringstream();
	ss << std::string((char*)data, size);
	Lexer lexer(&ss);

	auto parserPtr = new bison::Parser(lexer, *rootProg);
	parserPtr->parse();

	YAML::Emitter out;
	if(rootProg)
		rootProg->printYaml(out);
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
	parseFile(data, size);
	return 0;
}