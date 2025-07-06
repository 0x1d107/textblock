#include "input_handler.h"
#include "blocks.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
InputHandler::InputHandler(){
	init_commands();
}
void InputHandler::begin_block(std::stringstream  &liness){
	std::string name;
	liness >>name;
	if(block_handlers.count(name)){
		block_handlers[name]->begin(liness);
		block_stack.push_back(block_handlers[name].get());
	}
}

void InputHandler::end_block(std::stringstream &liness){
	block_stack.back()->end(liness);
	block_stack.pop_back();
}
void InputHandler::include_command(std::stringstream &liness){
	liness >>std::ws;
	std::string path = liness.str().substr(liness.tellg());
	auto fs = new std::ifstream(path);
	input_stack.push_back(fs);
}
void InputHandler::swap_input(std::stringstream &){
	size_t input_stack_size = input_stack.size();
	if(input_stack_size > 1){
		std::swap(input_stack[input_stack_size-1],input_stack[input_stack_size-2]);
	}
}
void InputHandler::setenv_command(std::stringstream &ss){
	std::string key;
	ss >> key;
	ss>>std::ws;
	std::string value = ss.str().substr(ss.tellg());
	if(setenv(key.c_str(),value.c_str(),1)){
		std::cerr << strerror(errno)<<std::endl;
		exit(1);
	}
}
void InputHandler::quote_command(std::stringstream &ss){
	ss >>std::ws;
	std::string cmd = cmd_prefix+ss.str().substr(ss.tellg());
	ss.str(cmd);

	if(block_stack.size())
		block_stack.back()->line(ss);
	else
		std::cout << cmd<<std::endl;
	
}
void InputHandler::init_commands(){
	//commands
	register_command("block",&InputHandler::begin_block);
	register_command("end",&InputHandler::end_block);
	register_command("include",&InputHandler::include_command);
	register_command("swap",&InputHandler::swap_input);
	register_command("q",&InputHandler::quote_command);
	register_command("setenv",&InputHandler::setenv_command);

	//blocks
	block_handlers["filter"] = std::make_unique<FilterBlock>();
	block_handlers["expand"] = std::make_unique<ExpansionBlock>(this);
}
void InputHandler::push_input(std::istream *input){
	input_stack.push_back(input);
}
void InputHandler::pop_input(){
	if(input_stack.back() != &std::cin)
		delete input_stack.back();
	input_stack.pop_back();
}
void InputHandler::process_input(){
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
		pop_input();
	}
}
