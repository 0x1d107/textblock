#include <deque>
#include <functional>
#include <iostream>
#include <memory>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include "blocks.h"

typedef std::function<void(std::stringstream &)> sshandler_t;
std::unordered_map<std::string,std::unique_ptr<TextBlock>> block_handlers;
std::deque<TextBlock *> block_stack;

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
std::deque<std::istream *> inputstack; 
void include_command(std::stringstream &liness){
	liness >>std::ws;
	std::string path = liness.str().substr(liness.tellg());
	auto fs = new std::ifstream(path);
	inputstack.push_back(fs);
}
void swap_input(std::stringstream &){
	size_t inputstack_size = inputstack.size();
	if(inputstack_size > 1){
		std::swap(inputstack[inputstack_size-1],inputstack[inputstack_size-2]);
	}
}

void init_commands( std::unordered_map<std::string,sshandler_t> &commands){
	commands["block"] = begin_block;
	block_handlers["filter"] = std::make_unique<FilterBlock>();
	commands["end"] = end_block;
	commands["include"] = include_command;
	commands["swap"] = swap_input; 
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
		inputstack.push_back(file);
	}else
		inputstack.push_back(&std::cin);
	const std::string cmd_prefix="#%";
	while(inputstack.size()){
		for(std::string line; std::getline(*inputstack.back(),line);){
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
		if(inputstack.back() != &std::cin)
			delete inputstack.back();
		inputstack.pop_back();
	}
}
