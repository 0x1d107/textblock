#include <cstring>
#include <deque>
#include <functional>
#include <iostream>
#include <memory>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <cstdlib>
#include "blocks.h"

typedef std::function<void(std::stringstream &)> sshandler_t;
const std::string cmd_prefix="#%";
std::unordered_map<std::string,std::unique_ptr<TextBlock>> block_handlers;
std::deque<TextBlock *> block_stack;
std::deque<std::istream *> input_stack; 

void begin_block(std::stringstream  &liness){
	std::string name;
	liness >>name;
	if(block_handlers.count(name)){
		block_handlers[name]->begin(liness);
		block_stack.push_back(block_handlers[name].get());
	}
}

void end_block(std::stringstream &liness){
	block_stack.back()->end(liness);
	block_stack.pop_back();
}
void include_command(std::stringstream &liness){
	liness >>std::ws;
	std::string path = liness.str().substr(liness.tellg());
	auto fs = new std::ifstream(path);
	input_stack.push_back(fs);
}
void swap_input(std::stringstream &){
	size_t input_stack_size = input_stack.size();
	if(input_stack_size > 1){
		std::swap(input_stack[input_stack_size-1],input_stack[input_stack_size-2]);
	}
}
void setenv_command(std::stringstream &ss){
	std::string key;
	ss >> key;
	ss>>std::ws;
	std::string value = ss.str().substr(ss.tellg());
	if(setenv(key.c_str(),value.c_str(),1)){
		std::cerr << strerror(errno)<<std::endl;
		exit(1);
	}
	
}
void quote_command(std::stringstream &ss){
	ss >>std::ws;
	std::string cmd = cmd_prefix+ss.str().substr(ss.tellg());
	ss.str(cmd);

	if(block_stack.size())
		block_stack.back()->line(ss);
	else
		std::cout << cmd<<std::endl;
	
}


void init_commands( std::unordered_map<std::string,sshandler_t> &commands){
	commands["block"] = begin_block;
	block_handlers["filter"] = std::make_unique<FilterBlock>();
	commands["end"] = end_block;
	commands["include"] = include_command;
	commands["swap"] = swap_input; 
	commands["q"] = quote_command;
	commands["setenv"] = setenv_command;
}


int main(int argc, const char **argv){
	std::unordered_map<std::string,sshandler_t>commands;
	init_commands(commands);
	if(argc >= 2){
		auto file = new std::ifstream(argv[1]);
		if(!file->is_open()){
			std::cerr << "failed to open file "<<argv[1]<<std::endl;
			exit(1);
		}
		input_stack.push_back(file);
	}else
		input_stack.push_back(&std::cin);
	while(input_stack.size()){
		for(std::string line; std::getline(*input_stack.back(),line);){
			std::stringstream liness(line);
			if(line.substr(0,cmd_prefix.size()) == cmd_prefix){
				liness.ignore(cmd_prefix.size());
				std::string cmd;
				liness >> cmd;
				if(commands.count(cmd))
					commands[cmd](liness);
			}else {
				if(block_stack.size())
					block_stack.back()->line(liness);
				else
					std::cout << line<<std::endl;
			}
		}
		if(input_stack.back() != &std::cin)
			delete input_stack.back();
		input_stack.pop_back();
	}
}
