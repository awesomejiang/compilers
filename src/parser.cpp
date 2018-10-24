#include "parser.h"

Parser::Parser(std::string const &input){
	int fd = open(input.c_str(), O_RDONLY);
}