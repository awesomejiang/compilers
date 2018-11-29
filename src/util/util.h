#ifndef UTIL_H
#define UTIL_H

#include <stdexcept>
#include <string>
#include <iostream>
#include <vector>

class CompileException: public std::runtime_error{
public:
	CompileException(std::string const &msg,
	  std::string const &file,
	  std::string const &func);

	void printError() const;

private:
	std::string const file, func;
};


struct CompileArgs{
	bool verbose = false;
	bool optimization = false;
	bool jit = false;
	bool ast = false;
	bool llvm = false;
	std::string input, output;
	std::vector<std::string> args;
};

CompileArgs setupArgs(int argc, char **argv);

void printUsage();

#endif