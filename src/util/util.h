#ifndef UTIL_H
#define UTIL_H

#include <stdexcept>
#include <string>
#include <iostream>
#include <vector>
#include <chrono>

class CompileException: public std::runtime_error{
public:
	CompileException(std::string const &msg,
	  std::string const &file,
	  std::string const &func);

	void printError() const;

private:
	std::string const file, func;
};


class Timer{
public:
	Timer(std::string name): name{name}, start(std::chrono::system_clock::now()) {}
	~Timer(){
		auto end = std::chrono::system_clock::now();
		auto t = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
		std::cout << name + " block took " + std::to_string(t) + " us." << std::endl;
	}
private:
	std::string name;
	std::chrono::system_clock::time_point start;
};

struct OptType{
	bool level3 = false;
	bool inlining = true;
	bool unrolling = true;
};

struct CompileArgs{
	bool verbose = false;
	bool optimization = false;
	bool jit = false;
	bool ast = false;
	bool llvm = false;
	std::string input, output;
	std::vector<std::string> args;
	OptType optType;
};	


CompileArgs setupArgs(int argc, char **argv);

void printUsage();

#endif