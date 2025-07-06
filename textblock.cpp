#include <iostream>
#include <fstream>
#include <cstdlib>
#include "input_handler.h"

int main(int argc, const char **argv){
	InputHandler input_handler;
	if(argc >= 2){
		auto file = new std::ifstream(argv[1]);
		if(!file->is_open()){
			std::cerr << "failed to open file "<<argv[1]<<std::endl;
			exit(1);
		}
		input_handler.push_input(file);
	}else
		input_handler.push_input(&std::cin);
	input_handler.variables["hello"] = "world";
	input_handler.process_input();
}
